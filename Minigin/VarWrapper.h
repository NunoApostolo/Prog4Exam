//variable wrapper, mainly used in Transform
#pragma once
template <typename T, class C>
class VarWrapper : public T{
	friend class Transform; // not sure if needed
public:
	VarWrapper() {}
	VarWrapper(C* pClass,T& (C::*getter)(), void (C::*setter)(T&)) {
		//this->var = var;
		this->pClass = pClass;
		this->getter = getter;
		this->setter = setter;
		this->csetter = nullptr;
		T::x = 0;
		T::y = 0;
		T::z = 0;
		cache = *this;
	}
	VarWrapper(C* pClass, T& (C::* getter)(), void (C::* setter)(const T&)) {
		//this->var = var;
		this->pClass = pClass;
		this->getter = getter;
		this->setter = nullptr;
		this->csetter = setter;
		T::x = 0;
		T::y = 0;
		T::z = 0;
		cache = *this;
	}
	VarWrapper(C* pClass, T& (C::* getter)(), void (C::* setter)(T&), void (C::* csetter)(const T&)) {
		//this->var = var;
		this->pClass = pClass;
		this->getter = getter;
		this->setter = setter;
		this->csetter = csetter;
		T::x = 0;
		T::y = 0;
		T::z = 0;
		cache = *this;
	}
	T& operator =(T& rhs) {
		if (setter != nullptr)
			(pClass->*setter)(rhs);
		else 
			(pClass->*csetter)(rhs);
		cache = rhs;
		return *this;
	}	
	T& operator =(const T& rhs) {
		cache = rhs;
		(pClass->*csetter)(rhs);
		return *this;
	}
	T& operator =(VarWrapper<T,C>& rhs) {
		if (setter != nullptr)
			(pClass->*setter)(rhs);
		else
			(pClass->*csetter)(rhs);
		cache = rhs;
		return *this;
	}

	template <typename T>
	void CheckCache(T val) { // check if axis was changed
		if (cache != val) {
			cache = val;
			if (setter != nullptr)
				(pClass->*setter)(val);
			else
				(pClass->*csetter)(val);
		}
	}

	T cache{};
private:
	C* pClass;

	T& (C::*getter)(); // function pointers to getters and setters, is getter unused?
	void (C::*setter)(T&);
	void (C::*csetter)(const T&);
};