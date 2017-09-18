#pragma once

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

template<typename T>
class UniWrapperFundamental
{
protected:
	// Wrapper related callbacks

	void _wrapperConstructed() const  // Wrapper constructed
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().wrapperConstructed();
	}

	void _wrapperDestructed() const  // Wrapper almost destructed
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().wrapperDestructed();
	}

	// Value related callbacks:

	// Universal callback for value related callbacks
	void _uniCallback(SetOfUniWrapperCallbacks setOfCallbacks) const
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback(setOfCallbacks, value);
	}

	// Value write
	void _valueConstructed() const  // Default and Copy constructors
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueConstructed(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueWritten(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({
			UniWrapperCallbacks::valueConstructed, UniWrapperCallbacks::valueWritten
		}, value);
	}

	void _valueAssigned() const  // operator=()
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueAssigned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueWritten(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().pointerToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().linkToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({
			UniWrapperCallbacks::valueAssigned,  
			UniWrapperCallbacks::valueWritten,
			UniWrapperCallbacks::pointerToValueReturned,
			UniWrapperCallbacks::linkToValueReturned
		}, value);
	}

	void _valueModified() const  // ++, +=, -=, etc.
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueModified(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueWritten(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().refferenceToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().linkToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({
			UniWrapperCallbacks::valueModified,  
			UniWrapperCallbacks::valueWritten,
			UniWrapperCallbacks::refferenceToValueReturned,
			UniWrapperCallbacks::linkToValueReturned
		}, value);
	}

	void _valueWritten() const // Any change
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueWritten(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ 
			UniWrapperCallbacks::valueWritten }, value);
	}

	// Value will be destructed
	void _valueDestructed() const
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueDestructed(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ 
			UniWrapperCallbacks::valueDestructed }, value);
	}

	// Read copy or refference
	void _refferenceToValueReturned() const  // Refference returned
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().refferenceToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().linkToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ 
			UniWrapperCallbacks::refferenceToValueReturned,  UniWrapperCallbacks::linkToValueReturned }, value);
	}

	void _constRefferenceToValueReturned() const  // Const refference returned
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().constRefferenceToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueWasRead(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().linkToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({
			UniWrapperCallbacks::constRefferenceToValueReturned,  
			UniWrapperCallbacks::valueWasRead,
			UniWrapperCallbacks::linkToValueReturned
		}, value);
	}

	void _pointerToValueReturned() const // Pointer to value returned
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().pointerToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().linkToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ 
			UniWrapperCallbacks::pointerToValueReturned,  UniWrapperCallbacks::linkToValueReturned }, value);
	}

	void _linkToValueReturned() const // Any type of links (Refference, Const Refference, Pointer) was returned from the class
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().linkToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ 
			UniWrapperCallbacks::linkToValueReturned }, value);
	}

	void _classConvertedToValueType() const // Copy of value was returned
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().classConvertedToValueType(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueWasRead(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ 
			UniWrapperCallbacks::classConvertedToValueType, UniWrapperCallbacks::valueWasRead }, value);
	}

	void _valueWasRead() const  // Any type of read (Copy or Link to the value)
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().valueWasRead(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({ UniWrapperCallbacks::valueWasRead }, value);
	}

	// Item from array value
	void _refferenceToTheItemOfTheArrayValueReturned() const  // operator[]. Value is an array. Refference to the value[index] was returned.
	{
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().refferenceToTheItemOfTheArrayValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().linkToValueReturned(value);
		Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks().uniCallback({
			UniWrapperCallbacks::refferenceToTheItemOfTheArrayValueReturned,
			UniWrapperCallbacks::linkToValueReturned
		}, value);
	}

public:
	UniWrapperFundamental(std::nullptr_t, std::nullptr_t, std::nullptr_t)
	{
	}

	UniWrapperFundamental()
	{
		_wrapperConstructed();
	}

	UniWrapperFundamental(const UniWrapperFundamental<T>& other)
	{
		_wrapperConstructed();
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
		_wrapperConstructed();
		_valueConstructed();
	}

	UniWrapperFundamental(UniWrapperFundamental<T>&& other)
		: UniWrapperFundamental()
	{
		_wrapperConstructed();
		swap(*this, other);
		_valueConstructed();
	}

	~UniWrapperFundamental()
	{
		_valueDestructed();
		_wrapperDestructed();
	}

	friend void swap(UniWrapperFundamental<T>& first, UniWrapperFundamental<T>& second)
	{
		first.value = second.value;
	}

	friend void swap(UniWrapperFundamental<T>& first, T second)
	{
		first.value = second;
	}

	UniWrapperFundamental<T>& operator=(UniWrapperFundamental<T> other) noexcept
	{
		if (this != std::addressof(other)) {
			swap(*this, other);
		}
		_valueAssigned();
		return *this;
	}

	UniWrapperFundamental<T>& operator=(T other) noexcept
	{
		swap(*this, other);
		_valueAssigned();
		return *this;
	}

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

	UniWrapperFundamental<T>& operator++(int) {
		UniWrapperFundamental<T> tmp(*this);
		this->operator++();
		return tmp;
	};

	UniWrapperFundamental<T>& operator--(int) {
		UniWrapperFundamental<T> tmp(*this);
		this->operator--();
		return tmp;
	};

protected:
	T value;
};

template<typename T> UniWrapperFundamental<T> operator+(UniWrapperFundamental<T> lOperand, UniWrapperFundamental<T> rOperand)
{
	return UniWrapperFundamental<T>(lOperand.getValueConstRef() + rOperand.getValueConstRef());
}

template<typename T> UniWrapperFundamental<T> operator-(UniWrapperFundamental<T> lOperand, UniWrapperFundamental<T> rOperand) {
	return UniWrapperFundamental<T>(lOperand.getValueConstRef() - rOperand.getValueConstRef());
}

template<typename T> UniWrapperFundamental<T> operator*(UniWrapperFundamental<T> lOperand, UniWrapperFundamental<T> rOperand) {
	return UniWrapperFundamental<T>(lOperand.getValueConstRef() * rOperand.getValueConstRef());
}

template<typename T> UniWrapperFundamental<T> operator/(UniWrapperFundamental<T> lOperand, UniWrapperFundamental<T> rOperand) {
	return UniWrapperFundamental<T>(lOperand.getValueConstRef() / rOperand.getValueConstRef());
}

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

template<typename T> UniWrapperFundamental<T> operator++(UniWrapperFundamental<T> lOperand, int) {
	UniWrapperFundamental<T> tmp(lOperand);
	lOperand.operator++();
	return tmp;
}

template<typename T> UniWrapperFundamental<T> operator--(UniWrapperFundamental<T> lOperand, int) {
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
	is >> obj.getValueRef();
	return is;
}

template<typename T> std::istream& operator>> (UniWrapperFundamental<T>& obj, std::istream& is)
{
	is >> obj.getValueRef();
	return is;
}

//==================================================================================

template<typename T>
class UniWrapperIntegral: public UniWrapperFundamental<T>
{
public:
	UniWrapperIntegral(std::nullptr_t, std::nullptr_t, std::nullptr_t)
	{
	}

	UniWrapperIntegral()
		: UniWrapperFundamental<T>(nullptr, nullptr, nullptr)
	{
		_wrapperConstructed();
	}

	UniWrapperIntegral(const UniWrapperIntegral<T>& other)
		: UniWrapperFundamental<T>(nullptr, nullptr, nullptr)
	{
		_wrapperConstructed();
		if (this != std::addressof(other)) {
			value = other.value;
		}
		_valueConstructed();
	}

	template< typename T >
	using constType = typename std::add_const<T>::type;
	UniWrapperIntegral(constType<T> value)
		: UniWrapperFundamental<T>(nullptr, nullptr, nullptr)
	{
		_wrapperConstructed();
		this->value = value;
		_valueConstructed();
	}

	UniWrapperIntegral(UniWrapperIntegral<T>&& other)
		: UniWrapperIntegral()
	{
		_wrapperConstructed();
		swap(*this, other);
		_valueConstructed();
	}

	~UniWrapperIntegral()
	{
		_valueDestructed();
		_wrapperDestructed();
	}

	UniWrapperIntegral<T>& operator=(UniWrapperIntegral<T> other) noexcept
	{
		if (this != std::addressof(other)) {
			swap(*this, other);
		}
		_valueAssigned();
		return *this;
	}

	UniWrapperIntegral<T>& operator=(T other) noexcept
	{
		swap(*this, other);
		_valueAssigned();
		return *this;
	}

	UniWrapperIntegral<T>& UniWrapperIntegral::operator%=(UniWrapperIntegral<T> rOperand) {
		value %= rOperand.value;
		_valueModified();
		return *this;
	}
};

template<typename T> UniWrapperIntegral<T> operator%(UniWrapperIntegral<T> lOperand, UniWrapperIntegral<T> rOperand) {
	return UniWrapperIntegral<T>(lOperand.getValueRef() % rOperand.getValueRef());
}

template<typename T> UniWrapperIntegral<T> operator%(const T& lOperand, UniWrapperIntegral<T> rOperand) {
	return UniWrapperIntegral<T>(lOperand % rOperand.getValueConstRef());
}

template<typename T> UniWrapperIntegral<T> operator%(UniWrapperIntegral<T> lOperand, const T& rOperand) {
	return UniWrapperIntegral<T>(lOperand.getValueConstRef() % rOperand);
}

//==================================================================================

template<typename T>
class UniWrapperPointer : public UniWrapperFundamental<T>
{
public:
	UniWrapperPointer(std::nullptr_t, std::nullptr_t, std::nullptr_t)
	{
	}

	UniWrapperPointer()
		: UniWrapperFundamental<T>(nullptr, nullptr, nullptr)
	{
		_wrapperConstructed();
	}

	UniWrapperPointer(const UniWrapperPointer<T>& other)
		: UniWrapperFundamental<T>(nullptr, nullptr, nullptr)
	{
		_wrapperConstructed();
		if (this != std::addressof(other)) {
			value = other.value;
		}
		_valueConstructed();
	}

	template< typename T >
	using constType = typename std::add_const<T>::type;
	UniWrapperPointer(constType<T> value)
		: UniWrapperFundamental<T>(nullptr, nullptr, nullptr)
	{
		_wrapperConstructed();
		this->value = value;
		_valueConstructed();
	}

	UniWrapperPointer(UniWrapperPointer<T>&& other)
		: UniWrapperPointer()
	{
		_wrapperConstructed();
		swap(*this, other);
		_wrapperConstructed();
	}

	~UniWrapperPointer()
	{
		_valueDestructed();
		_wrapperDestructed();
	}

	UniWrapperPointer<T>& operator=(UniWrapperPointer<T> other) noexcept
	{
		if (this != std::addressof(other)) {
			swap(*this, other);
		}
		_valueAssigned();
		return *this;
	}

	UniWrapperPointer<T>& operator=(T other) noexcept
	{
		swap(*this, other);
		_valueAssigned();
		return *this;
	}

	UniWrapperPointer<T>& operator+=(const std::ptrdiff_t rOperand) {
		value += rOperand;
		_valueModified();
		return *this;
	}

	UniWrapperPointer<T>& operator-=(const std::ptrdiff_t rOperand) {
		value -= rOperand;
		_valueModified();
		return *this;
	}
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
