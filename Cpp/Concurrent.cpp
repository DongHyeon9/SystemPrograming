#include "Concurrent.h"

void ThreadMain1()
{
	std::this_thread::sleep_for(std::chrono::milliseconds{ 100 });
	std::cout << "Thread Main 1" << std::endl;
}

void ThreadMain2(int Arg)
{
	std::this_thread::sleep_for(std::chrono::milliseconds{ 200 });
	std::cout << "Thread Main 2 : " << Arg << std::endl;
}

void ThreadMain3(int& Arg)
{
	std::this_thread::sleep_for(std::chrono::milliseconds{ 300 });
	Arg = 3;
	std::cout << "Thread Main 3 : " << Arg << std::endl;
}

void ThreadMain4(int Arg1, int Arg2)
{
	std::this_thread::sleep_for(std::chrono::milliseconds{ 400 });
	std::cout << "Thread Main 4 : " << Arg1 + Arg2 << std::endl;
}

int Add(int Lhs, int Rhs)
{
	std::cout << "Add" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds{ 100 });
	return Lhs + Rhs;
}

template<class T>
struct ReferenceWrapper
{
	T* obj{};
	ReferenceWrapper(T& Obj) :obj{ &Obj } {}
	operator T& () { return *obj; }
};

// std::ref
// 참조를 전달하는 함수
// 내부적으로 ReferenceWrapper를 사용중
template<class T>
ReferenceWrapper<T> Ref(T& Obj)
{
	return ReferenceWrapper<T>{Obj};
}

template<class T> requires std::is_integral_v<T>
void fill_vector(std::vector<T>& Vector, std::size_t Count = 10'000'000)
{
	std::mt19937 engine{ std::random_device{}() };
	std::uniform_int_distribution dist{ 0,100 };

	Vector.reserve(Count);

	for (std::size_t i = 0; i < Count; ++i)
		Vector.emplace_back(dist(engine));
}

void ExampleMain()
{
	// std::thread
	// 스레드를 생성하는 객체
	// join()으로 thread가 끝날 때까지 대기
	// detach()로 thread를 분리 할 수 있음
	// native_handle()로 해당 OS에서 지원하는 Handle을 가져올 수 있다
	// hardware_concurrency()로 물리적으로 지원하는 thread의 개수를 가져올 수 있다
	// MSVC에선 내부적으로 _beginthreadex를 호출하고 있다
	{
		int arg{ 10 };
		std::thread t1{ ThreadMain1 };
		std::thread t2{ ThreadMain2 ,2 };
		std::thread t3{ ThreadMain3 ,Ref(arg) };
		std::thread t4{ ThreadMain4 ,4 ,2 };

		t1.join();
		t2.join();
		t3.join();
		t4.join();
	}

	// std::promise, std::future
	// promise의 get_future로 future를 생성할 수 있다
	// 내부적으로 cv로 구현되어 있다
	// 복사 될 수 없다
	// future
	//  - get으로 promise의 값을 받아 올 수 있다(값이 없으면 대기)
	//  - wait_for로 무한정 대기 하지 않고 상태를 체크할 수 있다(std::future_status)
	//  - get은 한번만 호출이 가능하다
	//  - std::shared_future로 여러개의 future를 사용해 여러번 get할 수 있다
	// promise
	//  - get_future로 future를 생성할 수 있다
	//  - set_value_at_thread_exit, set_value로 값을 설정, future를 깨우고 값을 설정할 수 있다
	//  - std::promise<void>는 템플릿 특수화 되어있어 set_value에 인자를 안주고 future를 깨울 수 있다
	//  - set_exception_at_thread_exit, set_exception으로 예외를 던져 줄 수 있다
	{
		std::vector<int> v1{ 1,2,3,4,5,6,7,8,9,10 };
		std::vector<int> v2(10);

		std::promise<void> pm1{};
		std::future<void> ft1{ pm1.get_future() };

		std::promise<int> pm2{};
		std::future<int> ft2{ pm2.get_future() };

		std::thread t{ [&]() {
			std::partial_sum(v1.begin(),v1.end(),v2.begin());
			pm1.set_value();

			int s = std::accumulate(v2.begin(), v2.end(), 0);
			pm2.set_value(s);
		} };

		ft1.get();
		for (auto n : v2)
			std::cout << n << " ";

		const int ret{ ft2.get() };
		std::cout << '\n' << ret << std::endl;

		t.join();
	}

	// std::packaged_task
	// future와 promise를 사용하지 않은 일반 함수에 사용할 수 있는 함수 Wrapper
	// operator()로 가지고있는 함수를 실행 할 수 있다
	// get_future로 반환 값을 가져올 수 있다
	{
		std::packaged_task<int(int, int)> task{ Add };

		std::future<int> ft{ task.get_future() };

		std::thread t{ std::move(task),10,20 };

		const int ret{ ft.get() };
		std::cout << ret << std::endl;
		t.join();
	}

	// std::async
	// 주어진 함수를 비동기로 수행하는 함수 템플릿
	// 기존에 작성된 함수를 간단하게 스레드로 수행할 수 있다
	// 일반적인 구현은 스레드 풀을 사용
	// 반환값으로 std::future를 돌려줌
	// std::launch의 인자값으로 async의 실행을 정할 수 있음
	// |연산자를 통해 두개의 플래그를 동시에 사용할 수 있음(기본적으론 std::launch::async | std::launch::deferred으로 실행됨)
	// std::launch::async : 비동기로 실행(멀티스레드)
	// std::launch::deferred : future의 get이 호출될 때 함수를 실행
	// 아래 스펙은 async의 반환값일 때만 적용
	//  - future는 소멸자에서 get을 호출하기 때문에 반환값으로 받은 future가 소멸될 때 get을 자동으로 실행
	//  - 반환값을 받지 않은경우 임시객체의 소멸자가 호출되서 해당라인에서 실행됨
	{
		std::future<int> ft1 = std::async(Add, 10, 20);
		std::future<int> ft2 = std::async(std::launch::async, Add, 30, 40);
		std::future<int> ft3 = std::async(std::launch::deferred, Add, 50, 60);
		std::async(std::launch::deferred, Add, 70, 80);

		ft1.get();
		ft2.get();
		ft3.get();

		std::cout << "=====Async Test=====" << std::endl;

		using Vector = std::vector<std::size_t>;

		Vector v1{};
		Vector v2{};

		auto MultiThreadFunc = [&]()->long long
			{
				const std::size_t size{ v1.size() };
				const std::size_t firstIdx{ 0 };
				const std::size_t secondIdx{ size / 4 };
				const std::size_t thirdIdx{ size / 2 };
				const std::size_t fourthIdx{ size * 3 / 4 };
				auto ft1 = std::async([&] {return std::inner_product(v1.begin() + firstIdx, v1.begin() + secondIdx, v2.begin() + firstIdx, 0LL); });
				auto ft2 = std::async([&] {return std::inner_product(v1.begin() + secondIdx, v1.begin() + thirdIdx, v2.begin() + secondIdx, 0LL); });
				auto ft3 = std::async([&] {return std::inner_product(v1.begin() + thirdIdx, v1.begin() + fourthIdx, v2.begin() + thirdIdx, 0LL); });
				auto ft4 = std::async([&] {return std::inner_product(v1.begin() + fourthIdx, v1.end(), v2.begin() + fourthIdx, 0LL); });
				return ft1.get() + ft2.get() + ft3.get() + ft4.get();
			};

		auto SignleThreadFunc = [&]()->long long
			{
				return std::inner_product(v1.begin(), v1.end(), v2.begin(), 0LL);
			};

		auto Benchmark = [&](std::string_view Name, std::function<long long()> Func)
			{
				std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
				long long ret = Func();
				std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
				std::chrono::duration<double> timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
				std::cout << Name << " : " << ret << " / " << timeSpan.count() << "sec" << std::endl;
			};

		fill_vector(v1);
		fill_vector(v2);
		Benchmark("SingleThread", SignleThreadFunc);
		Benchmark("MultiThread", MultiThreadFunc);
	}

	// std::jthread ( C++ 20 )
	// RAII Idom이 적용된 thread
	// 소멸자에서 join을 호출해 준다
	// cooperatively interruptible joining thread(협력적으로 중단 가능한 스레드)
	// std::stop_token을 인자로 받아 token의 상태 값으로 중단할 수 있다
	// 함수에서 std::stop_token을 사용한다고 jthread에서 인자를 넘겨줄 필욘 없다
	{
		using namespace std::literals;

		auto CIJTFunc = [](std::stop_token token, int Cnt)
			{
				for (int i = 0; i < Cnt; ++i)
				{
					if (token.stop_requested())
					{
						std::cout << "Stop Response" << std::endl;
						return;
					}

					std::this_thread::sleep_for(500ms);
					std::printf("CIJT : %d\n", i);
				}
			};

		auto NonCIJTFunc = [](int Cnt)
			{
				for (int i = 0; i < Cnt; ++i)
				{
					std::this_thread::sleep_for(500ms);
					std::printf("NonCIJT : %d\n", i);
				}
			};

		std::jthread jt1{ CIJTFunc,10 };
		std::jthread jt2{ NonCIJTFunc,10 };

		std::this_thread::sleep_for(2s);

		jt1.request_stop();
		jt2.request_stop();
	}

	{
		// std::mutex
		// 공유자원에 대해 임계영역을 지정하는 클래스
		// lock과 unlock을 통해 임계영역을 지정
		// std::mutex, std::recursive_mutex, std::shared_mutex가 있으며, timed 버전도 있다
		// std::mutex : 기본형태 try_lock을 통해 lock을 걸 수 있는지 확인하고 가능하면 lock을 걸고 true를 리턴한다
		// std::recursive_mutex : 같은 스레드에 대해 다회 lock호출을 허용한다 단, lock횟수만큼 unlock을 호출해야 된다
		// std::shared_mutex : lock_shared와 unlock_shared를 추가적으로 지원, 읽기 쓰기에 대해 lock을 분리 할 수 있다
		// timed_mutex : try_lock_for, try_lock_until 함수를 통해 일정 시간동안, 혹은 일정 시간까지 lock을 시도할 수 있다
		//  - std::timed_mutex, std::recursive_timed_mutex, std::shared_timed_mutex
		// 
		// std::lock_guard
		// mutex에 RAII Idom을 적용시킬 수 있다
		// 가장 기본적인 형태로 생성자와 소멸자에서 각각 lock과 unlock을 호출한다
		// std::adopt_lock을 통해 생성자에서 lock을 호출하지 않을 수 있다
		// 
		// std::unique_lock
		// std::lock_guard에서 여러가지 추가기능을 넣었다
		// lock을 옮기는 move를 지원한다
		// std::adopt_lock : 생성자에서 lock을 걸지 않는다(이미 lock을 획득한 뮤텍스 관리)
		// std::try_to_lock : 생성자에서 try_lock을 사용, 락을 소유했는지 판별하기 위해 owns_lock 함수를 제공
		// std::defer_lock : 생성자에서 lock을 걸지 않는다(나중에 lock을 획득할 뮤텍스 관리)
		// std::chrono::time_point : 생성자에서 try_lock_until로 lock을 잡음
		// std::chrono::duration : 생성자에서 try_lock_for로 lock을 잡음
		// release() : unlock을 하지 않고 mutex의 소유권을 포기
		// operator bool = owns_lock()
		// 
		// std::lock
		// 두 개의 mutex를 내부적으로 deadlock이 걸리지 않도록 lock을 건다
		// RAII Idom이 적용되지 않아 직접 lock을 해제해 줘야 한다
		// 
		// std::scoped_lock ( C++ 17 )
		// 가변인자 템플릿으로 여러개의 락을 deadlock없이 순서대로 lock을 걸 수 있는 클래스
		// std::lock과는 다르게 RAII Idom이 적용되어 있다
		// 내부적으로 std::lock을 사용
		// 
		// std::shared_lock
		// shared_mutex에 대해 lock_shared를 관리하는 클래스
	}

	// std::condition_variable
	// wait를 호출하고 대기하던 스레드를 notify_one, notify_all 호출을 통해 깨울 수 있다
	// lost wake up을 방지하기위해 wait에 Predicate를 줄 수 있다
	{
		using namespace std::literals;

		std::condition_variable cv{};
		std::mutex mtx{};
		bool bIsReady{};
		bool bTerminate{};

		auto Consumer = [&](std::string_view Name)
			{
				while (!bTerminate)
				{
					{
						std::unique_lock<std::mutex> lock{ mtx };
						cv.wait(lock, [&] { return bIsReady || bTerminate; });
						bIsReady = false;
						std::this_thread::sleep_for(200ms);
					}
					std::printf("%s\n", Name.data());
				}
			};

		auto Producer = [&](std::stop_token Token)
			{
				while (true)
				{
					if (Token.stop_requested())
					{
						bTerminate = true;
						cv.notify_all();
						return;
					}
					{
						std::unique_lock<std::mutex> lock{ mtx };
						bIsReady = true;
					}
					std::printf("Producer\n");
					cv.notify_one();
					std::this_thread::sleep_for(40ms);
				}
			};

		constexpr std::size_t CONSUMER_COUNT{ 5 };
		std::jthread jt1{ Producer };
		std::vector<std::jthread> cosumers{};

		cosumers.reserve(CONSUMER_COUNT);
		for (std::size_t i = 0; i < CONSUMER_COUNT; ++i)
			cosumers.emplace_back(Consumer, std::string(1, static_cast<char>('A' + i)));

		std::this_thread::sleep_for(3s);

		jt1.request_stop();
	}

	// std::counting_semaphore
	// 특정 개수만큼 임계영역에 접근할 수 있는 동기화 기법(자원의 한정적인 공유)
	// std::counting_semaphore<최대 카운터> sem{카운터 초기값}의 형태로 생성
	// acquire(), release()로 접근과 해제를 관리
	// release()시 해제 개수를 지정할 수 있다. 단, 최대 카운터를 넘으면 예외발생
	// std::binary_semaphore = std::counting_semaphore<1>
	{
		using namespace std::literals;

		std::counting_semaphore<5> sem{ 5 };
		constexpr std::size_t LOOP_CNT{ 100 };
		constexpr std::size_t THREAD_CNT{ 10 };

		auto Job = [&](std::string_view Name)
			{
				sem.acquire();
				for (std::size_t i = 0; i < LOOP_CNT; ++i)
				{
					std::printf("%s", Name.data());
					std::this_thread::sleep_for(10ms);
				}
				std::printf("Finish %s\n", Name.data());
				sem.release();
			};

		std::vector<std::jthread> tv{};
		tv.reserve(THREAD_CNT);

		for (std::size_t i = 0; i < THREAD_CNT; ++i)
			tv.emplace_back(Job, std::string(1, static_cast<char>('A' + i)));
	}

	{
		// thread_locl은 static을 쓰지 않아도 암시적으로 static으로 변함
		// c++ 11버전부터 static 지역변수의 생성은 멀티스레드에 안전하다( ex. mayer's singleton )

		// std::call_once
		// 초기화 같은 작업을 한번만 수행 하고 싶을 때 사용
		// std::call_once(std::once_flag&, function, Args...)의 형태로 호출
		// std::once_flag는 복사와 이동 모두 삭제됨
	}

	// std::latch
	// 카운트 기반 동기화 기법
	// 한 번 밖에 쓸 수 없음
	// 설정한 개수만큼 count_down이 호출되면(count가 0이 되면) wait하고 있던 thread가 깨어난다
	// wait와 count_down이 합쳐진 arrive_and_wait이 있다
	// 
	// std::barrier
	// std::latch를 재사용 할 수 있게 만든 클래스
	// 생성자 인자에 콜백 함수를 등록할 수 있다(arrive_and_wait이나 count_down을 마지막으로 호출한 스레드가 호출한다)
	{
		std::latch complete{ 3 };
		std::latch gohome{ 1 };
		std::barrier barrier{ 3 ,[]() noexcept {std::printf("On Complete\n"); } };

		auto Company1 = [&](std::string_view Name)
			{
				std::printf("Start Work : %s\n", Name.data());
				std::printf("Finish Work : %s\n", Name.data());
				complete.count_down();
				gohome.wait();
				std::printf("Go Home : %s\n", Name.data());
			};

		auto Company2 = [&](std::string_view Name)
			{
				std::printf("Start Work : %s\n", Name.data());
				std::printf("Finish Work : %s\n", Name.data());

				barrier.arrive_and_wait();
				std::printf("Have Dinner : %s\n", Name.data());

				barrier.arrive_and_wait();
				std::printf("Go Home : %s\n", Name.data());
			};

		{
			std::jthread t1{ Company1 ,"Kim" }, t2{ Company1 ,"Lee" }, t3{ Company1 ,"Park" };

			complete.wait();
			std::printf("All Finish\n");
			gohome.count_down();
		}

		{
			std::jthread t1{ Company2 ,"Kim" }, t2{ Company2 ,"Lee" }, t3{ Company2 ,"Park" };
		}
	}

	// std::atomic
	// std::atomic<T>의 T가 복사계열 함수와 move 계열의 함수각 모두 trivial하지 않으면
	// static_assert에 걸린다
	// trivial하다 = 사용자가 만들지 않았다
	// 
	// memory order
	// 컴파일러는 컴파일 할 때 단일 스레드라 생각하고 최적화를 진행한다
	// 그래서 단일 스레드 기준 실행에 문제가 없다면 어셈블리 명령어를 재배치를 한다
	// 그리고 이러한 현상은 CPU 스케줄링에서도 나타난다
	// CPU 스케줄링은 명령에서 연산의 결과가 바뀌지 않는다면
	// 캐시메모리 혹은 레지스터에 있는 값부터 연산한다
	// 
	// std::memory_order
	// 위에서 말한 memory order의 문제를 해결할 수 있는 클래스
	// std::memory_order_relaxed : 가장 오버헤드가 적음, 원자성만 보장, reodering이 일어남
	// std::memory_order_acquire : load(읽기)연산에 적용, release와 같이 사용됨
	// std::memory_order_release : store(쓰기)연산에 적용, release 이전의 코드는 acquire 이후에 읽을 수 있다는것을 보장
	// std::memory_order_acq_rel : 읽기, 쓰기를 모두 수행하는 연산(ex.fetch_add)에 적용, acquire와 release를 합친 것 처럼 동작
	// std::memory_order_seq_cst : 가장 강한 메모리 오더, reodering을 진행하지 않음
	{
		// 메모리 오더때문에 탈출함
		int x{}, y{}, a{}, b{};

		// 탈출 못함
		//std::atomic<int> x{}, y{}, a{}, b{};
		//x.store(1, std::memory_order_release);
		//y.store(1, std::memory_order_release);
		//a.store(x.load(std::memory_order_acquire), std::memory_order_release);
		//b.store(y.load(std::memory_order_acquire), std::memory_order_release);

		volatile bool ready{};

		auto Thread1 = [&] {while (!ready); y = 1; a = x; };
		auto Thread2 = [&] {while (!ready); x = 1; b = y; };
		int count{};

		while (true)
		{
			++count;
			ready = false;
			x = y = a = b = 0;
			std::thread t1{ Thread1 };
			std::thread t2{ Thread2 };
			ready = true;

			t1.join();
			t2.join();

			if (a == 0 && b == 0)
				break;
		}
		std::cout << count << std::endl;
	}

	// std::atomic_flag
	// std::atomic<bool>과 유사
	// 모든 환경에서 lock_free를 보장
	// 최소한의 함수만 제공
	{
		// std::atomic_flag를 사용한 spinlock
		class SpinLock
		{
		public:
			void Lock() { while (flag.test_and_set()); }
			void Unlock() { flag.clear(); }
		private:
			std::atomic_flag flag{};
		};
	}

	// std::atomic_ref ( C++ 20 )
	// 참조처럼 동작하는데 원자적 연산을 보장하는 클래스
	{
		struct Foo { std::string name{}; int data{}; }foo;
		std::atomic_ref<int> cnt{ foo.data };
		for (int i = 0; i < 1'000'000; ++i) ++cnt;
	}

	// std::atomic<std::shared_ptr>> ( C++ 20 )
	// 참조 계수 증가/감소는 멀티스레드 에서 안전함( C++ 11 )
	// 하지만 대입에 대해선 안전하지 않음
	// 아토믹 스마트 포인터를 사용하면 대입에 대해서 안전해 질 수 있다
	{
		//atomic을 안쓰면 t1과 t2중 하나는 메모리 누수가 날 수도 있음
		std::atomic<std::shared_ptr<int>> ptr{ std::make_shared<int>(1) };
		std::thread t1{ [&ptr]() {ptr = std::make_shared<int>(2); } };
		std::thread t2{ [&ptr]() {ptr = std::make_shared<int>(3); } };
	}
}