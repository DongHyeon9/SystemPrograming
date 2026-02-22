#include "SharedPtr.h"

// std::shared_ptr의 문제 (비상속 스마트 포인터)
// 참조 계수를 관리하는 객체가 2개 이상 생성될 수 있다
// 
// ex)
// Object* Obj = new Object;
// std::shared_ptr<Object> sp1(Obj);
// std::shared_ptr<Object> sp2(Obj);

// template hoisting
class RefCountBase
{
public:
	void AddRef() const
	{
		count.fetch_add(1, std::memory_order_relaxed);
	}

protected:
	~RefCountBase() { std::println("~RefCountBase"); }

protected:
	mutable std::atomic<int> count{};
};

template<class T>
class RefCount : public RefCountBase
{
public:
	void Release() const
	{
		int ret = count.fetch_sub(1, std::memory_order_acq_rel);

		if (ret == 1)
			delete static_cast<T*>(const_cast<RefCount*>(this));
	}
};

template<class T>
class SharedPtr
{
public:
	explicit SharedPtr(T* Ptr = nullptr) :pObj{ Ptr } { if (pObj) pObj->AddRef(); }
	SharedPtr(const SharedPtr<T>& Ptr) :pObj{ Ptr.pObj } { if (pObj) pObj->AddRef(); }
	~SharedPtr() { if (pObj) pObj->Release(); }
private:
	T* pObj;
};

class Foo : public RefCount<Foo>
{
public:
	~Foo() { std::println("~Foo"); }
};

void ExampleMain()
{
	SharedPtr<Foo> f1{ new Foo };
	SharedPtr<Foo> f2 = f1;
}