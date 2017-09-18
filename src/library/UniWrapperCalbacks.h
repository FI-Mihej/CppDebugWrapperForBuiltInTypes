#pragma once

#include <map>
#include <set>

//==================================================================================

enum UniWrapperCallbacks {
	// Wrapper related
	wrapperConstructed,  // Wrapper object constructed
	wrapperDestructed,  // Wrapper object destructed

	// Write
	valueConstructed,  // Default and Copy constructors
	valueAssigned,  // operator=()
	valueModified,  // ++, +=, -=, etc.
	valueWritten,  // Any change

	// Value will be destructed
	valueDestructed,

	// Refference returned
	refferenceToValueReturned,
	pointerToValueReturned,
	linkToValueReturned,  // Any type of reference returned (Ref, Const Ref, Ptr)

	// Read
	constRefferenceToValueReturned,  // Const Ref was returned
	classConvertedToValueType, // Copy was returned
	valueWasRead, // Any type of read (Copy, Const Ref)

	// Item from array value
	refferenceToTheItemOfTheArrayValueReturned  // operator[]. Value is an array. Refference to the value[index] was returned.
};

typedef std::set<UniWrapperCallbacks> SetOfUniWrapperCallbacks;

class UniWrapperCallbacksNames
{
public:
	UniWrapperCallbacksNames();
	~UniWrapperCallbacksNames();

	const std::string& operator()(UniWrapperCallbacks id);

protected:
	std::map<UniWrapperCallbacks, std::string> names;
};

//==================================================================================

template<typename T>
class UniWrapperCalbacks
{
public:
	UniWrapperCalbacks() {};
	virtual ~UniWrapperCalbacks() {};

	// CALLBACKS

	// Wrapper related callbacks
	virtual void wrapperConstructed() {};  // Wrapper object constructed
	virtual void wrapperDestructed() {};  // Wrapper object destructed

	// Value related callbacks:

	// Universal callback for value related callbacks
	virtual void uniCallback(SetOfUniWrapperCallbacks setOfCallbacks, T value) {};

	// Write
	virtual void valueConstructed(T value) {};  // Default and Copy constructors
	virtual void valueAssigned(T value) {};  // operator=()
	virtual void valueModified(T value) {};  // ++, +=, -=, etc.
	virtual void valueWritten(T value) {};  // Any change

	// Value will be destructed
	virtual void valueDestructed(T value) {};

	// Refference returned
	virtual void refferenceToValueReturned(T value) {};  // Refference returned
	virtual void pointerToValueReturned(T value) {};  // Pointer to value returned
	virtual void linkToValueReturned(T value) {};  // Any type of reference returned (Ref, Const Ref, Ptr)	

	// Read
	virtual void constRefferenceToValueReturned(T value) {};  // Const Ref was returned
	virtual void classConvertedToValueType(T value) {};  // Copy of the value was returned
	virtual void valueWasRead(T value) {};  // Any type of read (Copy, Const Ref)

	// Item from array value
	virtual void refferenceToTheItemOfTheArrayValueReturned(T value) {};  // Value is an array. Refference to the value[index] was returned.

protected:
	UniWrapperCallbacksNames callbackNames;
};

//==================================================================================

template<typename T>
class UniWrapperCalbacksHolder
{
public:
	UniWrapperCalbacksHolder() 
		:callbacks(nullptr)
	{}

	~UniWrapperCalbacksHolder()
	{}

	UniWrapperCalbacks<T>& getCallbacks()
	{
		if (callbacks)
			return *callbacks;
		else
			return defaultCallbacks;
	}

	void setCallbacks(UniWrapperCalbacks<T>& callbacks)
	{
		this->callbacks = &callbacks;
	}

	void clearCallbacks()
	{
		this->callbacks = nullptr;
	}

protected:
	UniWrapperCalbacks<T> defaultCallbacks;
	UniWrapperCalbacks<T>* callbacks = nullptr;
};

//==================================================================================

template<typename T>
class CallbacksChanger
{
public:
	CallbacksChanger(UniWrapperCalbacks<T>* newCallbacks, bool needToDeleteOnDestroy = false)
		:needToDeleteOnDestroy(needToDeleteOnDestroy)
	{
		oldCallbacks = &(Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks());
		Singleton<UniWrapperCalbacksHolder<T> >().setCallbacks(*newCallbacks);
	}

	~CallbacksChanger()
	{
		UniWrapperCalbacks<T>* newCallbacks = &(Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks());
		Singleton<UniWrapperCalbacksHolder<T> >().clearCallbacks();
		Singleton<UniWrapperCalbacksHolder<T> >().setCallbacks(*oldCallbacks);
		if (needToDeleteOnDestroy)
			delete newCallbacks;
	}

protected:
	UniWrapperCalbacks<T>* oldCallbacks;
	bool needToDeleteOnDestroy = false;
};