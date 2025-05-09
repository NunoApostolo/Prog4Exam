#pragma once
//variable wrapper, mainly used in Transform
#include <functional>
template <typename T, class C>
class VarWrapper : public T{
public:
	friend class Transform;

	VarWrapper() {}
	VarWrapper(C* pClass, std::function<void()> getter) {}

	VarWrapper(C* pClass, std::function<T()> getter, std::function<void(const T&)> setter, T init = T{}) {
		//this->var = var;
		this->pClass = pClass;
		this->getter = getter;
		this->setter = setter;
		//this->csetter = nullptr;
		T::x = init.x;
		T::y = init.y;
		T::z = init.z;
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
	const T operator +(VarWrapper<T, C>& rhs) {
		T t{ getter() + rhs.getter() };
		return t;
	}
	const T operator +(const T& rhs) {
		T t{ getter() + rhs };
		return t;
	}
	const T& operator+=(const Vector3& rhs) {
		setter(Vector3(x + rhs.x, y + rhs.y, z + rhs.z));
		return *this;
	}
	const T operator -(VarWrapper<T, C>& rhs) {
		//setter();
		T t{ getter() - rhs.getter() };
		return t;
	}
	const T operator -(const T& rhs) {
		//setter(getter() - rhs);
		T t{ getter() - rhs };
		return t;
	}
	const T& operator-=(const Vector3& rhs) {
		setter(Vector3(x - rhs.x, y - rhs.y, z - rhs.z));
		return *this;
	}
	template <typename T>
	void CheckCache(T val) { // check if axis was changed
		if (cache != val) {
			cache = val;
			setter(val);
		}
	}

	void SetVec3(glm::vec3 vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
		cache = vec;
	}
	void SetVec3(Vector3 vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
		cache = vec;
	}

	T cache{};
protected:

private:
	C* pClass;

	std::function<T()> getter;
	std::function<void(const T&)> setter;
	//std::function<void(const T&)> csetter;

	//T& (C::*getter)(); // function pointers to getters and setters, is getter unused?
	//void (C::*setter)(T&);
	//void (C::*csetter)(const T&);

	using T::x;
	using T::y;
	using T::z;
};
