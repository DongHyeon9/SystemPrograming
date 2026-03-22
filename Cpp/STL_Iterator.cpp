#include "STL_Iterator.h"

// STL Iterator 종류
// 1. Input Iterator : 한 방향으로만 순회, 읽기 전용
// 2. Output Iterator : 한 방향으로만 순회, 쓰기 전용
// 3. Forward Iterator : 한 방향으로만 순회, 읽기/쓰기 모두 가능
// 4. Bidirectional Iterator : 양방향으로 순회, 읽기/쓰기 모두 가능
// 5. Random Access Iterator : 양방향으로 순회, 읽기/쓰기 모두 가능, 임의 접근 가능
//
// Multiple Pass Guarantee
// 동일한 범위를 여러 번 순회할 수 있다는 것을 의미
// Forward Iterator 이상에서 보장되는 특성
// Forward Iterator 이상에서는 한 번 순회한 후에도 다시 처음부터 순회할 수 있다
// 반면, Input Iterator와 Output Iterator는 한 번 순회하면 다시 처음부터 순회할 수 없을 수 있다
// ex) stream에서 사용하는 istream_iterator는 한 번 순회하면 다시 처음부터 순회할 수 없다

void ExampleMain()
{
	// std::begin, std::end
	// 배열과 컨테이너 모두에서 사용할 수 있음
	// 일관된 인터페이스 제공
	{
		std::println("===== std::begin, std::end =====");
		int arr[] = { 1, 2, 3, 4, 5 };
		for(auto it = std::begin(arr); it != std::end(arr); ++it)
		{
			std::print("{} ", *it);
		}
		std::println("");
	}

	// std::next, std::prev
	// iterator를 이동시킴
	// 원본 iterator는 변경하지 않고 새로운 iterator를 반환
	{
		std::println("===== std::next, std::prev =====");
		std::vector<int> vec{ 1, 2, 3, 4, 5 };
		auto iter = vec.begin() + 2;
		auto nextIter = std::next(iter, 2);
		auto prevIter = std::prev(nextIter, 1);

		std::println("Current: {}, Next: {}, Prev: {}", *iter, *nextIter, *prevIter);
	}

	// std::advance
	// iterator를 이동시킴
	// 원본 iterator 자체를 변경
	{
		std::println("===== std::advance =====");
		std::vector<int> vec{ 1, 2, 3, 4, 5 };
		auto iter = vec.begin();
		std::advance(iter, 3);
		std::println("Advanced Iterator: {}", *iter);
	}

	// const_iterator
	// 컨테이너의 요소를 읽기 전용으로 접근할 수 있는 반복자
	{
		std::println("===== const_iterator =====");
		std::vector<int> vec{ 1, 2, 3, 4, 5 };
		// std::const_iterator<std::vector<int>::iterator>
		// std::vector<int>::const_iterator
		for (auto it = vec.cbegin(); it != vec.cend(); ++it)
		{
			std::print("{} ", *it);
		}
		std::println("");
	}

	// reverse_iterator
	// 컨테이너의 요소를 역방향으로 순회할 수 있는 반복자
	{
		std::println("===== reverse_iterator =====");
		std::vector<int> vec{ 1, 2, 3, 4, 5 };
		// std::reverse_iterator<std::vector<int>::iterator>
		// std::vector<int>::reverse_iterator
		for (auto it = vec.rbegin(); it != vec.rend(); ++it)
		{
			std::print("{} ", *it);
		}
		std::println("");
	}

	// const_reverse_iterator
	// 컨테이너의 요소를 역방향으로 순회하면서 읽기 전용으로 접근할 수 있는 반복자
	{
		std::println("===== const_reverse_iterator =====");
		std::vector<int> vec{ 1, 2, 3, 4, 5 };
		// std::const_reverse_iterator<std::vector<int>::iterator>
		// std::vector<int>::const_reverse_iterator
		for (auto it = vec.crbegin(); it != vec.crend(); ++it)
		{
			std::print("{} ", *it);
		}
		std::println("");
	}

	// move_iterator
	// 이동 시맨틱을 활용하여 요소를 이동할 때 사용하는 반복자
	{
		std::println("===== move_iterator =====");
		std::vector<std::string> src{ "Hello", "World", "C++" };
		std::vector<std::string> dst{};
		// std::move_iterator<std::vector<std::string>::iterator>
		std::move(std::make_move_iterator(src.end()), std::make_move_iterator(src.end()), std::back_inserter(dst));
		std::println("Src : {}", src);
		std::println("Dst : {}", dst);
	}

	// istream_iterator, ostream_iterator
	// 입력 스트림에서 요소를 읽거나 출력 스트림에 요소를 쓸 때 사용하는 반복자
	// Single Pass Guarantee
	{
		std::println("===== istream_iterator, ostream_iterator =====");
		std::istringstream iss("1 2 3 4 5");
		std::vector<int> vec{ std::istream_iterator<int>(iss), std::istream_iterator<int>() };
		std::println("Vector from istream: {}", vec);
		std::ostringstream oss;
		std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(oss, " "));
		std::println("Output stream: {}", oss.str());
	}

	// back_inserter, front_inserter
	// 시퀀스 컨테이너에 요소를 추가할 때 사용할 수 있는 반복자 어댑터
	// STL 알고리즘은 컨테이너에 Push하지 않음
	// 추가할려면 위의 반복자 어댑터를 사용해야 함
	// back_inserter : push_back을 사용하여 요소를 추가
	// front_inserter : push_front을 사용하여 요소를 추가
	{
		std::println("===== back_inserter, front_inserter =====");
		std::deque<int> src{ 1, 2, 3, 4, 5 };
		std::deque<int> dst{};
		std::copy(src.begin(), src.begin() + 2, std::back_inserter(dst));
		std::copy(src.begin() + 3, src.end(), std::front_inserter(dst));

		std::println("Src : {}", src);
		std::println("Dst : {}", dst);
	}

	// std::counted_iterator ( C++ 20 )
	// 기존 반복자에 개수를 관리하는 기능을 추가한 반복자 어댑터
	// 한 개의 반복자 안에 범위의 끝을 나타내는 개수를 함께 관리
	// 특정 범위 내에서 요소를 순회할 때 유용
	{
		std::println("===== std::counted_iterator =====");
		std::vector<int> vec{ 1, 2, 3, 4, 5 };
		std::counted_iterator<std::vector<int>::iterator> countedIter{ vec.begin(), 3 };
		while (countedIter.count() != 0)
		{
			std::print("{} ", *countedIter++);
		}
		std::println("");
	}

	// std::default_sentinel ( C++ 20 )
	// 범위의 끝을 알 수 있는 반복자(std::counted_iterator 등)의 last로 사용할 수 있는 객체
	{
		std::println("===== std::counted_iterator =====");
		std::vector<int> vec{ 1, 2, 3, 4, 5 };
		std::counted_iterator countedIter{ vec.begin(), 3 };
		while (countedIter != std::default_sentinel)
		{
			std::print("{} ", *countedIter++);
		}
		std::println("");
	}

	// std::common_iterator ( C++ 20 )
	// 서로 다른 유형의 반복자를 하나의 범위로 사용할 수 있도록 하는 반복자 어댑터
	// C++ 20 이전에 똑같은 Iterator를 받는 알고리즘들을 위해 지원
	// ex) std::counted_iterator와 std::default_sentinel을 하나의 범위로 사용할 수 있음
	{
		std::println("===== std::common_iterator =====");
		std::vector<int> vec{ 1, 2, 3, 4, 5 };
		std::counted_iterator countedIter{ vec.begin(), 3 };
		using T = std::common_iterator<std::counted_iterator<std::vector<int>::iterator>, std::default_sentinel_t>;

		auto ret = std::find(T{ countedIter }, T{ std::default_sentinel }, 2);
		if (ret != T{ std::default_sentinel })
		{
			std::println("Found: {}", *ret);
		}
		else
		{
			std::println("Not Found");
		}
	}

	// std::sentinel_for<S, I> ( C++ 20 )
	// S가 I의 sentinel로 사용 가능한지 조사할 때 사용하는 concept
	{
		std::println("===== std::sentinel_for =====");
		using vi_t = std::vector<int>::iterator;
		using ci_t = std::counted_iterator<vi_t>;

		constexpr bool Sentinel_CountedIter = std::sentinel_for<std::default_sentinel_t, ci_t>;
		constexpr bool Sentinel_VectorIter = std::sentinel_for<std::default_sentinel_t, vi_t>;
		constexpr bool VectorIter_VectorIter = std::sentinel_for<vi_t, vi_t>;

		std::println("std::sentinel_for<std::default_sentinel_t, ci_t>: {}", Sentinel_CountedIter);
		std::println("std::sentinel_for<std::default_sentinel_t, vi_t>: {}", Sentinel_VectorIter);
		std::println("std::sentinel_for<vi_t, vi_t>: {}", VectorIter_VectorIter);
	}

	// std::ranges::subranges ( C++ 20 )
	// iterator와 sentinel을 사용해
	// 특정컨테이너의 특정구간을 정의하는 반복자를 만듬
	// [first, last)로 묶여있음
	// tuple처럼 사용가능
	{
		std::println("===== std::ranges::subranges =====");
		std::vector<int> v{ 1, 2, 3, 4, 5 };
		auto sub = std::ranges::subrange(v.begin(), v.begin() + 2);
		std::println("sub : {}", sub);
		auto [first, last] = sub;
		std::println("sum : {}", std::accumulate(first, last, 0));
	}
}