//variable wrapper, mainly used in Transform
#include <functional>
#pragma once
template <typename T, class C>
class VarWrapper : public T{
	friend class Transform; // not sure if needed
public:
	VarWrapper() {}
	VarWrapper(C* pClass, std::function<void()> getter) {}

	VarWrapper(C* pClass, std::function<T()> getter, std::function<void(const T&)> setter) {
		//this->var = var;
		this->pClass = pClass;
		this->getter = getter;
		this->setter = setter;
		//this->csetter = nullptr;
		T::x = 0;
		T::y = 0;
		T::z = 0;
		cache = *this;
	}
	//VarWrapper(C* pClass, std::function<T()> getter, std::function<void(const T&)> setter) {
	//	//this->var = var;
	//	this->pClass = pClass;
	//	this->getter = getter;
	//	this->setter = nullptr;
	//	this->csetter = setter;
	//	T::x = 0;
	//	T::y = 0;
	//	T::z = 0;
	//	cache = *this;
	//}
	//VarWrapper(C* pClass, std::function<T()> getter, std::function<void(T&)> setter, std::function<void(const T&)> csetter) {
	//	//this->var = var;
	//	this->pClass = pClass;
	//	this->getter = getter;
	//	this->setter = setter;
	//	this->csetter = csetter;
	//	T::x = 0;
	//	T::y = 0;
	//	T::z = 0;
	//	cache = *this;
	//}
	T& operator =(T& rhs) {
		cache = rhs;
		setter(rhs);

		return *this;
	}	
	T& operator =(const T& rhs) {
		cache = rhs;
		setter(rhs);

		//(pClass->*csetter)(rhs);
		return *this;
	}
	T& operator =(VarWrapper<T,C>& rhs) {
		cache = rhs;
		setter(rhs.getter());

		//if (setter != nullptr)
		//	setter(rhs);
		//	//(pClass->*setter)(rhs);
		//else
		//	csetter(rhs);
		//	//(pClass->*csetter)(rhs);
		return *this;
	}
	T& operator +(VarWrapper<T, C>& rhs) {
		setter(getter() + rhs.getter());
		return *this;
	}
	T& operator -(VarWrapper<T, C>& rhs) {
		setter(getter() - rhs.getter());
		return *this;
	}
	template <typename T>
	void CheckCache(T val) { // check if axis was changed
		if (cache != val) {
			cache = val;
			setter(val);
		}
	}

	T cache{};
private:
	C* pClass;

	std::function<T()> getter;
	std::function<void(const T&)> setter;
	//std::function<void(const T&)> csetter;

	//T& (C::*getter)(); // function pointers to getters and setters, is getter unused?
	//void (C::*setter)(T&);
	//void (C::*csetter)(const T&);
};