#include "STL_Algorithm.h"

struct Point { int x, y; };

template<>
struct std::formatter<Point>
{
	constexpr auto parse(std::format_parse_context& Ctx)
	{
		// 아무 옵션 안씀
		return Ctx.begin();
	}

	// 실제 출력
	template<class FC>
	auto format(const Point& P, FC& Ctx) const
	{
		return std::format_to(Ctx.out(), "({}, {})", P.x, P.y);
	}
};

// forward
// 완벽 전달을 위한 함수
// std::forward는 전달받은 인자가 lvalue인지 rvalue인지에 따라 적절하게 전달하는 함수
// - lvalue는 lvalue로, rvalue는 rvalue로 전달

// move
// std::move는 전달받은 인자를 rvalue로 변환하는 함수
// - lvalue든 rvalue든 상관없이 rvalue로 전달

// Invoke
// 함수 객체를 호출하는 함수
// 일반적으로 완벽전달을 통해 함수 내부에서 호출용으로 사용함
template<class F, class ... ARGS>
decltype(auto) InvokeFunc(F&& Func, ARGS&& ... Args)
{
	return std::invoke(std::forward<F>(Func), std::forward<ARGS>(Args)...);
}

void ExampleMain()
{
	// Bind
	// 함수 객체를 생성하는 함수
	// 함수와 인자를 바인딩하여 새로운 함수 객체를 생성
	{
		std::println("===== Bind, Invoke =====");
		auto AddFunc = std::bind([](int A, int B) {return A + B; }, std::placeholders::_1, std::placeholders::_2);
		std::println("{}", InvokeFunc(AddFunc, 1, 2));
	}

	// Transform
	// 범위 내 원소들 각각에 대해 인자로 전달받을 함수를 실행 한 후, 그 결과를 Iterator로 전달받은 위치에 저장하는 함수
	// 입력 범위와 출력 범위는 겹칠 수 있음
	// 
	// 단항함수 버전
	// (First, Last, DstFirst, UnaryFunc)
	// First부터 Last까지의 범위에 대해 UnaryFunc을 실행한 후, 그 결과를 DstFirst부터 시작하는 범위에 저장
	//
	// 이항 함수	버전
	// (First1, Last1, First2, DstFirst, BinaryFunc)
	// First1부터 Last1까지의 범위와 First2부터 시작하는 범위에 대해 BinaryFunc을 실행한 후, 그 결과를 DstFirst부터 시작하는 범위에 저장
	{
		std::println("===== Transform Unaray =====");
		std::vector<int> in1{ 1, 2, 3, 4, 5 };
		std::vector<int> in2(in1.size());
		std::transform(in1.begin(), in1.end(), in2.begin(), [](int Value) { return Value * Value; });
		std::println("in1 : {}", in1);
		std::println("in2 : {}", in2);

		std::println("===== Transform Binary =====");
		std::vector<int> out(in1.size());
		std::transform(in1.begin(), in1.end(), in2.begin(), out.begin(), [](int A, int B) { return A + B; });
		std::println("out : {}", out);
	}

	// Distance
	// 반복자 사이의 거리를 계산하는 함수
	// 반환 값은 ptrdiff_t 타입
	{
		std::println("===== Distance =====");
		std::vector<int> v{ 1, 2, 3, 4, 5 };
		auto dist = std::distance(v.begin(), v.end());
		std::println("Distance : {}", dist);
	}

	// Accumulate
	// 컨테이너의 모든 원소에 대해 이항 함수를 적용하여 하나의 결과를 반환하는 함수
	{
		std::println("===== Accumulate =====");
		std::vector<int> v{ 1, 2, 3, 4, 5 };
		auto sum = std::accumulate(v.begin(), v.end(), 0);
		auto fac = std::accumulate(v.begin(), v.end(), 1, [](int Lhs, int Rhs) {return Lhs * Rhs; });
		std::println("sum : {}", sum);
		std::println("fac : {}", fac);
	}

	// for_each
	// 범위 내 원소들 각각에 대해 인자로 전달받을 함수를 실행하는 함수
	{
		std::println("===== for_each =====");
		std::vector<int> v{ 1, 2, 3, 4, 5 };
		std::for_each(v.begin(), v.end(), [](int& Value) { Value *= Value; });
		std::println("v : {}", v);
	}

	// remove - erase
	// 컨테이너에서 특정 값과 일치하는 원소들을 제거하는 함수
	// remove는 제거할 원소들을 컨테이너의 뒤쪽으로 이동시키고, 제거된 원소들의 뒤쪽에 새로운 end를 반환
	// erase는 remove가 반환한 새로운 end부터 컨테이너의 끝까지의 원소들을 실제로 제거
	// 
	// remove는 원소들을 제거하는 것이 아니라, 제거할 원소들을 컨테이너의 뒤쪽으로 이동시키는 함수
	// 따라서 remove를 호출한 후에는 컨테이너의 크기가 변경되지 않는다
	// 대신, remove가 반환하는 새로운 end부터 컨테이너의 끝까지의 원소들은 제거되야 할 원소들임
	// 따라서 remove를 호출한 후에는 erase를 사용하여 실제로 제거된 원소들을 컨테이너에서 제거해야 한다
	//
	// 같은 함수더라도 맴버 함수로 제공하는 알고리즘이 있다면
	// 그게 더 최적화된 버전이므로 그걸 사용하는게 좋다
	{
		std::println("===== remove - erase =====");
		std::vector<int> v{ 1, 2, 3, 4, 5, 3, 6 };
		std::list<int> l{ 1, 2, 3, 4, 5, 3, 6 };

		v.erase(std::remove(v.begin(), v.end(), 3), v.end());
		l.remove(3);

		std::println("v : {}", v);
		std::println("l : {}", l);
	}

	// copy
	// 요소들을 복사하는 함수
	// copy_n
	//  - n개만큼 복사하는 함수
	// copy_backward
	//  - 복사한 것을 뒤에부터 채워넣는 함수(순서는 유지됨)
	// fill, move도 지원한다
	// fill -> 특정 값으로 범위를 채우는 함수
	// move -> copy가 아닌 이동을 하는 함수
	{
		std::println("===== copy =====");
		std::vector<int> v{ 1, 2, 3, 4, 5 };
		std::vector<int> copy(v.size());

		std::copy_n(v.begin(), 3, copy.begin());
		std::println("copy_n : {}", copy);

		copy.clear();
		std::copy(v.begin(), v.end(), std::back_inserter(copy));
		std::println("copy : {}", copy);

		copy.clear();
		copy.resize(7, 0);
		std::copy_backward(v.begin(), v.end(), copy.end());
		std::println("copy_backward : {}", copy);

	}

	// xxx_copy
	// 연산의 결과를 다른 컨테이너에 저장
	{
		std::println("===== xxx_copy =====");
		std::vector<int> v1{ 1, 2, 3, 4, 5, 3, 6 };
		std::vector<int> v2(v1.size(), 0);
		v2.erase(std::remove_copy(v1.begin(), v1.end(), v2.begin(), 3), v2.end());
		
		std::println("v1 : {}", v1);
		std::println("v2 : {}", v2);
	}

	// find
	// 요소를 탐색하는 알고리즘
	// 이 또한 맴버함수 버전이 있다면 맴버함수 버전을 호출하는것을 권장
	// first_of와 last_of버전으로 찾는 시작 위치를 정할 수 있다
	{
		std::println("===== find =====");
		std::string s{ "asdf|qwer|zxcv" };
		std::println("find : {}", s.find("|"));
		std::println("find_first_of : {}", s.find_first_of("|"));
		std::println("find_last_of : {}", s.find_last_of('|'));
	}

	// xxx_if
	// 해당 조건에 맞는 요소를 찾는 함수
	// not 버전도 있다 (xxx_if_not)
	// copy와 섞어서 사용할 수 있다
	{
		std::println("===== xxx_if =====");
		std::vector<int> v1{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		std::vector<int> v2(v1.size(), 0);

		v2.erase(std::remove_copy_if(v1.begin(), v1.end(), v2.begin(), [](int Val) {return Val & 1; }));
		std::println("v1 : {}", v1);
		std::println("v2 : {}", v2);
	}

	// count
	// 범위 안의 원소들 중 특정 값과 일치하는 개수를 센다
	// count_if
	// 특정 조건을 만족하는 원소의 개수를 센다
	{
		std::println("===== count =====");
		std::vector<int> v1{ 1, 2, 1, 4, 1, 6, 1, 8, 1 };
		std::println("count : {}", std::count(v1.begin(), v1.end(), 1));
		std::println("count_if : {}", std::count_if(v1.begin(), v1.end(), [](int Val) {return !(Val & 1); }));
	}

	// binary_search
	// 정렬된 컨테이너에 대해 이진 탐색을 수행한다
	{
		std::println("===== binary_search =====");
		std::vector<int> v{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		std::println("2 : {}", std::binary_search(v.begin(), v.end(), 2));
		std::println("50 : {}", std::binary_search(v.begin(), v.end(), 50));
		std::println("5 : {}", std::binary_search(v.begin(), v.end(), 5));
		std::println("10 : {}", std::binary_search(v.begin(), v.end(), 10));
		std::println("9 : {}", std::binary_search(v.begin(), v.end(), 9));
	}

	// equal_range
	// 정렬된 범위에서 특정 값의 첫번째와 마지막 위치를 반환 -> std::pair<iter, iter>
	{
		std::println("===== equal_range =====");
		std::vector<int> v{ 1, 2, 3, 4, 5, 5, 5, 5, 5, 5, 6, 7, 8, 9 };
		auto range = std::equal_range(v.begin(), v.end(), 5);
		std::println("first : {}", std::distance(v.begin(), range.first));
		std::println("last : {}", std::distance(v.begin(), range.second));
		std::println("range : {}", std::distance(range.first, range.second));
	}

	// xxx_bound
	// 범위 안의 원소들 중 상한선과 하한선이 만족되는 첫번째 원소를 반환
	// lower_bound -> value보다 작지 않은 첫번째 원소 (하한선)
	// upper_bound -> value보다 크지 않은 첫번째 원소 (상한선)
	{
		std::println("===== xxx_bound =====");
		std::vector<int> v{ 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
		auto upper = std::upper_bound(v.begin(), v.end(), 3);
		auto lower = std::lower_bound(v.begin(), v.end(), 3);
		std::println("upper : {}, {}", *upper, std::distance(std::begin(v), upper));
		std::println("lower : {}, {}", *lower, std::distance(std::begin(v), lower));
	}

	// sort
	// 정렬하는 알고리즘
	// 내부적으로 introsort를 사용하고 있다(n log(n))
	// stable_sort -> 동일한 값들 사이의 상대적인 순서를 유지
	// partial_sort -> N번째 까지만 정렬하고 나머지는 정렬하지 않음 ( ex. 상위 5등 ), 상대적 순서 유지 X
	{
		std::println("===== partial_sort =====");
		std::array<int, 10> arr{ 6, 4, 9, 7, 3, 5, 8, 1, 2, 0 };
		std::partial_sort(arr.begin(), arr.begin() + 5, arr.end());
		std::println("arr : {}", arr);
	}

	// predicate concept ( C++20 )
	// predicate의 요구사항을 정의한 컨셉 ( bool 또는 bool로 변환 가능한 값을 반환하는 함수 )
	// C++ 20이전까지 find와 find_if는 인자의 개수와 형태가 동일한 템플릿이라 오버로딩이 불가
	{
		std::println("===== predicate concept =====");
		auto func1 = [](int) {return true; };
		auto func2 = [](int, int) {return false; };
		auto func3 = [](double a) {return a; };
		auto func4 = [](int a) {std::println("{}", a); };

		std::println("{}", std::predicate<decltype(func1), int>);
		std::println("{}", std::predicate<decltype(func2), int, int>);
		std::println("{}", std::predicate<decltype(func3), double>);
		std::println("{}", std::predicate<decltype(func4), int>);
	}

	// constrained ( C++20 )
	// concept으로 제약된 알고리즘
	// iterator 대신 컨테이너를 받을 수 있음
	// 기존 코드와 호환성을 위해 std::ranges안에 함수객체로 만들어짐
	// 함수 객체로 만듬으로써 using namespace를 사용했을 때 ADL(Argument Dependent Lookup)을 피해감(ADL은 함수끼리만 적용)
	// 기존에 사용하던 함수들을 ranges 네임스페이스를 써서 사용할 수 있다 
	// ex) std::find -> std::ranges::find

	// Projection ( C++20 )
	// 비교하기 전에 변환하는 함수
	// 알고리즘의 컴테이너 버전 뿐 아니라 반복자 버전도 사용 가능
	{
		std::println("===== Projection =====");
		{
			std::vector<std::string> v{ "AAAA", "D", "BB", "CCC" };
			std::ranges::sort(v);
			std::println("사전 순 : {}", v);

			// 오름차순
			std::ranges::sort(v, {}, & std::string::size);
			std::println("길이 오름차순 : {}", v);

			// 내림차순
			std::ranges::sort(v, std::greater{}, & std::string::size);
			std::println("길이 내림차순 : {}", v);
		}

		{
			std::vector<Point> v{ {1,1}, {2,2}, {3,3}, {4,4}, {5,5}, };
			auto ret1 = std::ranges::find(v, 2, &Point::y);
			auto ret2 = std::ranges::find(v.begin(), v.end(), 3, &Point::y);
			auto ret3 = std::ranges::find_if(v, [](int Val) {return Val == 4; }, &Point::y);

			std::println("ret1 : {}", *ret1);
			std::println("ret2 : {}", *ret2);
			std::println("ret3 : {}", *ret3);
		}
	}
}