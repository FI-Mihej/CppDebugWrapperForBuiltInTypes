// UniWrapperForBuiltInTypes.cpp : Defines the entry point for the console application.
//
#include <type_traits>
#include <iostream>
#include <exception>
#include <cstddef>
#include "Singleton.hpp"
#include "UniWrapperCalbacks.h"


class NotImplementedException : public std::exception
{
	virtual const char* what() const throw()
	{
		return "Not implemented";
	}
};

//==================================================================================

//template<typename T>
//class IntDivAppropriate
//{
//public:
//	T value(T a, T b)
//	{
//		return a % b;
//	}
//
//	void link(T& a, T& b)
//	{
//		a %= b;
//	}
//};
//
//
//template<typename T>
//class IntDivNotAppropriate
//{
//public:
//	T value(T& a, T& b)
//	{
//		throw NotImplementedException();
//		return a;
//	}
//
//	void link(T& a, T& b)
//	{
//		throw NotImplementedException();
//	}
//};
//
//////==================================================================================
////
////template<typename T>
////class IntPlusAppropriate
////{
////public:
////	T value(T a, T b)
////	{
////		return a + b;
////	}
////
////	void link(T& a, T& b)
////	{
////		a += b;
////	}
////};
////
////
////template<typename T>
////class IntPlusNotAppropriate
////{
////public:
////	T value(T& a, T& b)
////	{
////		throw NotImplementedException();
////		return a;
////	}
////
////	void link(T& a, T& b)
////	{
////		throw NotImplementedException();
////	}
////};
////
//////==================================================================================
////
////template<typename T>
////class IntMinuxAppropriate
////{
////public:
////	T value(T a, T b)
////	{
////		return a + b;
////	}
////
////	void link(T& a, T& b)
////	{
////		a += b;
////	}
////};
////
////
////template<typename T>
////class IntMinusNotAppropriate
////{
////public:
////	T value(T& a, T& b)
////	{
////		throw NotImplementedException();
////		return a;
////	}
////
////	void link(T& a, T& b)
////	{
////		throw NotImplementedException();
////	}
////};
//
////==================================================================================
//
//template<typename T>
//class ArrayAccessAppropriate
//{
//	using ItemRefType = typename std::add_lvalue_reference<typename std::remove_pointer<typename T>::type>::type;
//
//	ItemRefType item(T pointer, std::size_t idx)
//	{
//		return pointer[idx];
//	}
//};
//
//
//template<typename T>
//class ArrayAccessNotAppropriate
//{
//	using ItemRefType = typename std::add_lvalue_reference<typename std::remove_pointer<typename T>::type>::type;
//
//	ItemRefType item(T pointer, std::size_t idx)
//	{
//		throw NotImplementedException();
//		return pointer;
//	}
//};

//==================================================================================

template<typename T>
class UniWrapperFundamental
{
protected:
	void _uniCallback(SetOfUniWrapperCallbacks setOfCallbacks) const
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback(setOfCallbacks, value);
	}

	void _valueConstructed() const
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueConstructed(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueWritten(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ UniWrapperCallbacks::valueConstructed,  UniWrapperCallbacks::valueWritten }, value);
	}
	void _valueAssigned() const
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueAssigned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueWritten(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ UniWrapperCallbacks::valueAssigned,  UniWrapperCallbacks::valueWritten }, value);
	}
	void _valueModified() const
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueModified(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueWritten(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ UniWrapperCallbacks::valueModified,  UniWrapperCallbacks::valueWritten }, value);
	}
	void _valueWritten() const // Any change
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueWritten(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ UniWrapperCallbacks::valueWritten }, value);
	}

	void _valueDestructed() const
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueDestructed(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ UniWrapperCallbacks::valueDestructed }, value);
	}

	void _classConvertedToValueType() const // Read
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().classConvertedToValueType(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ UniWrapperCallbacks::classConvertedToValueType }, value);
	}

	void _refferenceToValueReturned() const
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().refferenceToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().linkToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ UniWrapperCallbacks::refferenceToValueReturned,  UniWrapperCallbacks::linkToValueReturned }, value);
	}
	void _pointerToValueReturned() const
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().pointerToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().linkToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ UniWrapperCallbacks::pointerToValueReturned,  UniWrapperCallbacks::linkToValueReturned }, value);
	}
	void _linkToValueReturned() const // Any type of links was returned from the class
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().linkToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ UniWrapperCallbacks::linkToValueReturned }, value);
	}

	void _constRefferenceToValueReturned() const
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().constRefferenceToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueWasRead(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ UniWrapperCallbacks::constRefferenceToValueReturned,  UniWrapperCallbacks::valueWasRead }, value);
	}
	void _valueReaden() const
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueWasRead(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ UniWrapperCallbacks::valueWasRead }, value);
	}

	void _refferenceToTheItemOfTheArrayValueReturned() const  // Value is an array. Refference to the value[index] was returned.
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().refferenceToTheItemOfTheArrayValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ UniWrapperCallbacks::refferenceToTheItemOfTheArrayValueReturned }, value);
	}

public:
	UniWrapperFundamental(std::nullptr_t, std::nullptr_t, std::nullptr_t)
	{
	}
	UniWrapperFundamental()
	{
		_valueConstructed();
	}
	UniWrapperFundamental(const UniWrapperFundamental<T>& other)
	{
		if (this != std::addressof(other)) {
			value = other.value;
		}
		_valueConstructed();
	}
	template< typename T >
	using constType = typename std::add_const<T>::type;
	UniWrapperFundamental(constType<T> value)
		:value(value)
	{
		_valueConstructed();
	}

	~UniWrapperFundamental()
	{
		_valueDestructed();
	}

	UniWrapperFundamental<T>& operator=(const UniWrapperFundamental<T>& other)
	{
		if (this != std::addressof(other)) {
			value = other.value;
		}
		_valueAssigned();
		return *this;
	}
	UniWrapperFundamental<T>& operator=(const T& other)
	{
		value = other;
		_valueAssigned();
		return *this;
	}

	//using ItemRefType = typename std::add_lvalue_reference<typename std::remove_pointer<typename T>::type>::type;

	//ItemRefType operator[](std::size_t idx)
	//{
	//	typedef std::conditional<std::is_pointer<T>::value, ArrayAccessAppropriate<T>, ArrayAccessNotAppropriate<T> >::type ArrayAccess;
	//	ArrayAccess arrayAccess;
	//	_refferenceToTheItemOfTheArrayValueReturned();
	//	return arrayAccess.item(value, idx);
	//	//return value[idx];
	//}
	//const ItemRefType operator[](std::size_t idx) const
	//{
	//	typedef std::conditional<std::is_pointer<T>::value, ArrayAccessAppropriate<T>, ArrayAccessNotAppropriate<T> >::type ArrayAccess;
	//	ArrayAccess arrayAccess;
	//	_refferenceToTheItemOfTheArrayValueReturned();
	//	return arrayAccess.item(value, idx);
	//	//return value[idx];
	//}

	using PointerType = typename std::add_pointer<typename T>::type;

	PointerType operator&() throw()
	{
		_pointerToValueReturned();
		return std::addressof(value);
	}

	operator T() const
	{
		_classConvertedToValueType();
		return value;
	}

	using RefType = typename std::add_lvalue_reference<typename T>::type;

	RefType getValueRef()
	{
		_refferenceToValueReturned();
		return value;
	}

	const RefType& getValueConstRef()
	{
		_constRefferenceToValueReturned();
		return value;
	}

	UniWrapperFundamental<T>& UniWrapperFundamental::operator+=(UniWrapperFundamental<T> rOperand) {
		value += rOperand.value;
		_valueModified();
		return *this;
	}
	UniWrapperFundamental<T>& UniWrapperFundamental::operator-=(UniWrapperFundamental<T> rOperand) {
		value -= rOperand.value;
		_valueModified();
		return *this;
	}
	UniWrapperFundamental<T>& UniWrapperFundamental::operator*=(UniWrapperFundamental<T> rOperand) {
		value *= rOperand.value;
		_valueModified();
		return *this;
	}
	UniWrapperFundamental<T>& UniWrapperFundamental::operator/=(UniWrapperFundamental<T> rOperand) {
		value /= rOperand.value;
		_valueModified();
		return *this;
	}
	//UniWrapperFundamental<T>& UniWrapperFundamental::operator%=(UniWrapperFundamental<T> rOperand) {
	//	typedef std::conditional<std::is_integral<T>::value, IntDivAppropriate<T>, IntDivNotAppropriate<T> >::type IntDiv;

	//	IntDiv intDiv;
	//	intDiv.link(value, rOperand.value);
	//	//value %= rOperand.value;
	//	_valueModified();
	//	return *this;
	//}

	//UniWrapperFundamental<T>& UniWrapperFundamental::operator+=(const T& rOperand) {
	//	value += rOperand;
	//	return *this;
	//}
	//UniWrapperFundamental<T>& UniWrapperFundamental::operator-=(const T& rOperand) {
	//	value -= rOperand;
	//	return *this;
	//}
	//UniWrapperFundamental<T>& UniWrapperFundamental::operator*=(const T& rOperand) {
	//	value *= rOperand;
	//	return *this;
	//}
	//UniWrapperFundamental<T>& UniWrapperFundamental::operator/=(const T& rOperand) {
	//	value /= rOperand;
	//	return *this;
	//}
	//UniWrapperFundamental<T>& UniWrapperFundamental::operator%=(std::add_const<T>::type& rOperand) {
	//	typedef std::conditional<std::is_integral<T>::value, IntDivAppropriate<T>, IntDivNotAppropriate<T> >::type IntDiv;

	//	IntDiv intDiv;
	//	intDiv.link(value, rOperand);
	//	//value %= rOperand;
	//	return *this;
	//}

	UniWrapperFundamental<T>& UniWrapperFundamental::operator++() {
		++value;
		_valueModified();
		return *this;
	};
	UniWrapperFundamental<T>& UniWrapperFundamental::operator--() {
		--value;
		_valueModified();
		return *this;
	};

protected:
	T value;
};

template<typename T> UniWrapperFundamental<T> operator+(UniWrapperFundamental<T> lOperand, UniWrapperFundamental<T> rOperand)
{
	return UniWrapperFundamental(lOperand.getValueConstRef() + rOperand.getValueConstRef());
};
template<typename T> UniWrapperFundamental<T> operator-(UniWrapperFundamental<T> lOperand, UniWrapperFundamental<T> rOperand) {
	return UniWrapperFundamental<T>(lOperand.getValueConstRef() - rOperand.getValueConstRef());
}
template<typename T> UniWrapperFundamental<T> operator*(UniWrapperFundamental<T> lOperand, UniWrapperFundamental<T> rOperand) {
	return UniWrapperFundamental<T>(lOperand.getValueConstRef() * rOperand.getValueConstRef());
}
template<typename T> UniWrapperFundamental<T> operator/(UniWrapperFundamental<T> lOperand, UniWrapperFundamental<T> rOperand) {
	return UniWrapperFundamental<T>(lOperand.getValueConstRef() / rOperand.getValueConstRef());
}
//template<typename T> UniWrapperFundamental<T> operator%(UniWrapperFundamental<T> lOperand, UniWrapperFundamental<T> rOperand) {
//	typedef std::conditional<std::is_integral<T>::value, IntDivAppropriate<T>, IntDivNotAppropriate<T> >::type IntDiv;
//
//	IntDiv intDiv;
//	return UniWrapperFundamental<T>(intDiv.value(lOperand.getValueConstRef(), rOperand.getValueConstRef()));
//	//return UniWrapperFundamental<T>(lOperand.getValueRef() % rOperand.getValueRef());
//}

template<typename T> UniWrapperFundamental<T> operator+(const T& lOperand, UniWrapperFundamental<T> rOperand) {
	return UniWrapperFundamental<T>(lOperand + rOperand.getValueConstRef());
}
template<typename T> UniWrapperFundamental<T> operator-(const T& lOperand, UniWrapperFundamental<T> rOperand) {
	return UniWrapperFundamental<T>(lOperand - rOperand.getValueConstRef());
}
template<typename T> UniWrapperFundamental<T> operator*(const T& lOperand, UniWrapperFundamental<T> rOperand) {
	return UniWrapperFundamental<T>(lOperand * rOperand.getValueConstRef());
}
template<typename T> UniWrapperFundamental<T> operator/(const T& lOperand, UniWrapperFundamental<T> rOperand) {
	return UniWrapperFundamental<T>(lOperand / rOperand.getValueConstRef());
}
//template<typename T> UniWrapperFundamental<T> operator%(const T& lOperand, UniWrapperFundamental<T> rOperand) {
//	typedef std::conditional<std::is_integral<T>::value, IntDivAppropriate<T>, IntDivNotAppropriate<T> >::type IntDiv;
//
//	IntDiv intDiv;
//	return UniWrapperFundamental<T>(intDiv.value(lOperand, rOperand.getValueConstRef()));
//	//return UniWrapperFundamental<T>(lOperand % rOperand.getValueConstRef());
//}

template<typename T> UniWrapperFundamental<T> operator+(UniWrapperFundamental<T> lOperand, const T& rOperand) {
	return UniWrapperFundamental<T>(lOperand.getValueConstRef() + rOperand);
}
template<typename T> UniWrapperFundamental<T> operator-(UniWrapperFundamental<T> lOperand, const T& rOperand) {
	return UniWrapperFundamental<T>(lOperand.getValueConstRef() - rOperand);
}
template<typename T> UniWrapperFundamental<T> operator*(UniWrapperFundamental<T> lOperand, const T& rOperand) {
	return UniWrapperFundamental<T>(lOperand.getValueConstRef() * rOperand);
}
template<typename T> UniWrapperFundamental<T> operator/(UniWrapperFundamental<T> lOperand, const T& rOperand) {
	return UniWrapperFundamental<T>(lOperand.getValueConstRef() / rOperand);
}
//template<typename T> UniWrapperFundamental<T> operator%(UniWrapperFundamental<T> lOperand, const T& rOperand) {
//	typedef std::conditional<std::is_integral<T>::value, IntDivAppropriate<T>, IntDivNotAppropriate<T> >::type IntDiv;
//
//	IntDiv intDiv;
//	return UniWrapperFundamental<T>(intDiv.value(lOperand.getValueConstRef(), rOperand));
//	//return UniWrapperFundamental<T>(lOperand.getValueConstRef() % rOperand);
//}

template<typename T> UniWrapperFundamental<T> operator++(UniWrapperFundamental<T> lOperand, int) {
	//T val = lOperand.getValueConstRef();
	//++lOperand;
	//return UniWrapperFundamental<T>(val);
	UniWrapperFundamental<T> tmp(lOperand);
	lOperand.operator++();
	return tmp;
};

template<typename T> UniWrapperFundamental<T> operator--(UniWrapperFundamental<T> lOperand, int) {
	//T val = lOperand.getValueConstRef();
	//--lOperand;
	//return UniWrapperFundamental<T>(val);
	UniWrapperFundamental<T> tmp(lOperand);
	lOperand.operator--();
	return tmp;
}

template<typename T> std::ostream& operator<<(std::ostream& os, UniWrapperFundamental<T>& obj)
{
	os << obj.getValueConstRef();
	return os;
}
template<typename T> std::ostream& operator<<(UniWrapperFundamental<T>& obj, std::ostream& os)
{
	os << obj.getValueConstRef();
	return os;
}
template<typename T> std::istream& operator>> (std::istream& is, UniWrapperFundamental<T>& obj)
{
	//// read obj from stream
	//if ( /* T could not be constructed */)
	//	is.setstate(std::ios::failbit);
	is >> obj.getValueRef();
	return is;
}
template<typename T> std::istream& operator>> (UniWrapperFundamental<T>& obj, std::istream& is)
{
	//// read obj from stream
	//if ( /* T could not be constructed */)
	//	is.setstate(std::ios::failbit);
	is >> obj.getValueRef();
	return is;
}

//==================================================================================

template<typename T>
class UniWrapperIntegral: public UniWrapperFundamental<T>
{
public:
	//using UniWrapperFundamental<T>::UniWrapperFundamental;

	UniWrapperIntegral(std::nullptr_t, std::nullptr_t, std::nullptr_t)
	{
	}
	UniWrapperIntegral()
		: UniWrapperFundamental(nullptr, nullptr, nullptr)
	{
		_valueConstructed();
	}
	UniWrapperIntegral(const UniWrapperIntegral<T>& other)
		: UniWrapperFundamental(nullptr, nullptr, nullptr)
	{
		if (this != std::addressof(other)) {
			value = other.value;
		}
		_valueConstructed();
	}
	template< typename T >
	using constType = typename std::add_const<T>::type;
	UniWrapperIntegral(constType<T> value)
		: UniWrapperFundamental(nullptr, nullptr, nullptr)
	{
		this->value = value;
		_valueConstructed();
	}

	~UniWrapperIntegral()
	{
		_valueDestructed();
	}

	UniWrapperIntegral<T>& UniWrapperIntegral::operator%=(UniWrapperIntegral<T> rOperand) {
		//typedef std::conditional<std::is_integral<T>::value, IntDivAppropriate<T>, IntDivNotAppropriate<T> >::type IntDiv;

		//IntDiv intDiv;
		//intDiv.link(value, rOperand.value);
		value %= rOperand.value;
		_valueModified();
		return *this;
	}
};

template<typename T> UniWrapperIntegral<T> operator%(UniWrapperIntegral<T> lOperand, UniWrapperIntegral<T> rOperand) {
	//typedef std::conditional<std::is_integral<T>::value, IntDivAppropriate<T>, IntDivNotAppropriate<T> >::type IntDiv;

	//IntDiv intDiv;
	//return UniWrapperIntegral<T>(intDiv.value(lOperand.getValueConstRef(), rOperand.getValueConstRef()));
	return UniWrapperIntegral<T>(lOperand.getValueRef() % rOperand.getValueRef());
}

template<typename T> UniWrapperIntegral<T> operator%(const T& lOperand, UniWrapperIntegral<T> rOperand) {
	//typedef std::conditional<std::is_integral<T>::value, IntDivAppropriate<T>, IntDivNotAppropriate<T> >::type IntDiv;

	//IntDiv intDiv;
	//return UniWrapperIntegral<T>(intDiv.value(lOperand, rOperand.getValueConstRef()));
	return UniWrapperIntegral<T>(lOperand % rOperand.getValueConstRef());
}

template<typename T> UniWrapperIntegral<T> operator%(UniWrapperIntegral<T> lOperand, const T& rOperand) {
	//typedef std::conditional<std::is_integral<T>::value, IntDivAppropriate<T>, IntDivNotAppropriate<T> >::type IntDiv;

	//IntDiv intDiv;
	//return UniWrapperIntegral<T>(intDiv.value(lOperand.getValueConstRef(), rOperand));
	return UniWrapperIntegral<T>(lOperand.getValueConstRef() % rOperand);
}

//==================================================================================

template<typename T>
class UniWrapperPointer : public UniWrapperFundamental<T>
{
public:
	//using UniWrapperFundamental<T>::UniWrapperFundamental;

	UniWrapperPointer(std::nullptr_t, std::nullptr_t, std::nullptr_t)
	{
	}
	UniWrapperPointer()
		: UniWrapperFundamental(nullptr, nullptr, nullptr)
	{
		_valueConstructed();
	}
	UniWrapperPointer(const UniWrapperIntegral<T>& other)
		: UniWrapperFundamental(nullptr, nullptr, nullptr)
	{
		if (this != std::addressof(other)) {
			value = other.value;
		}
		_valueConstructed();
	}
	template< typename T >
	using constType = typename std::add_const<T>::type;
	UniWrapperPointer(constType<T> value)
		: UniWrapperFundamental(nullptr, nullptr, nullptr)
	{
		this->value = value;
		_valueConstructed();
	}
	//UniWrapperPointer(const std::ptrdiff_t value)
	//	: UniWrapperFundamental(nullptr, nullptr, nullptr)
	//{
	//	value = static_cast<T>(value);
	//	_valueConstructed();
	//}

	~UniWrapperPointer()
	{
		_valueDestructed();
	}

	//operator std::ptrdiff_t() const
	//{
	//	_classConvertedToValueType();
	//	return static_cast<std::ptrdiff_t>(value);
	//}

	//using ItemRefType = typename std::add_lvalue_reference<typename std::remove_pointer<typename T>::type>::type;

	//ItemRefType operator[](std::size_t idx)
	//{
	//	//typedef std::conditional<std::is_pointer<T>::value, ArrayAccessAppropriate<T>, ArrayAccessNotAppropriate<T> >::type ArrayAccess;
	//	//ArrayAccess arrayAccess;
	//	_refferenceToTheItemOfTheArrayValueReturned();
	//	//return arrayAccess.item(value, idx);
	//	return value[idx];
	//}
	//const ItemRefType operator[](std::size_t idx) const
	//{
	//	//typedef std::conditional<std::is_pointer<T>::value, ArrayAccessAppropriate<T>, ArrayAccessNotAppropriate<T> >::type ArrayAccess;
	//	//ArrayAccess arrayAccess;
	//	_refferenceToTheItemOfTheArrayValueReturned();
	//	//return arrayAccess.item(value, idx);
	//	return value[idx];
	//}
};

template<typename T> UniWrapperPointer<T> operator+(const std::ptrdiff_t lOperand, UniWrapperPointer<T> rOperand) {
	return UniWrapperPointer<T>(lOperand + rOperand.getValueConstRef());
}
template<typename T> UniWrapperPointer<T> operator-(const std::ptrdiff_t lOperand, UniWrapperPointer<T> rOperand) {
	return UniWrapperPointer<T>(lOperand - rOperand.getValueConstRef());
}
template<typename T> UniWrapperPointer<T> operator*(const std::ptrdiff_t lOperand, UniWrapperPointer<T> rOperand) {
	return UniWrapperPointer<T>(lOperand * rOperand.getValueConstRef());
}
template<typename T> UniWrapperPointer<T> operator/(const std::ptrdiff_t lOperand, UniWrapperPointer<T> rOperand) {
	return UniWrapperPointer<T>(lOperand / rOperand.getValueConstRef());
}

template<typename T> UniWrapperPointer<T> operator+(UniWrapperPointer<T> lOperand, const std::ptrdiff_t rOperand) {
	return UniWrapperPointer<T>(lOperand.getValueConstRef() + rOperand);
}
template<typename T> UniWrapperPointer<T> operator-(UniWrapperPointer<T> lOperand, const std::ptrdiff_t rOperand) {
	return UniWrapperPointer<T>(lOperand.getValueConstRef() - rOperand);
}
template<typename T> UniWrapperPointer<T> operator*(UniWrapperPointer<T> lOperand, const std::ptrdiff_t rOperand) {
	return UniWrapperPointer<T>(lOperand.getValueConstRef() * rOperand);
}
template<typename T> UniWrapperPointer<T> operator/(UniWrapperPointer<T> lOperand, const std::ptrdiff_t rOperand) {
	return UniWrapperPointer<T>(lOperand.getValueConstRef() / rOperand);
}

//==================================================================================

template<typename T>
using UniWrapperChoseIntegralFundamental = typename std::conditional<std::is_integral<T>::value, UniWrapperIntegral<T>, UniWrapperFundamental<T> >::type;

template<typename T>
using UniWrapperChosePointerIntegralFundamental = typename std::conditional<std::is_pointer<T>::value, UniWrapperPointer<T>, UniWrapperChoseIntegralFundamental<T> >::type;

template<typename T>
class UniWrapperUniversal : public UniWrapperChosePointerIntegralFundamental<T>
{
public:
	using UniWrapperChosePointerIntegralFundamental<T>::UniWrapperChosePointerIntegralFundamental;
};

