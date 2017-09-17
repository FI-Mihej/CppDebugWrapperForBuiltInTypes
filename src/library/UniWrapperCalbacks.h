#pragma once

#include <map>
#include <set>

//==================================================================================

enum UniWrapperCallbacks {
	wrapperConstructed,
	valueConstructed,
	valueAssigned,
	valueModified,
	valueWritten,

	valueDestructed,
	wrapperDestructed,

	classConvertedToValueType,

	refferenceToValueReturned,
	pointerToValueReturned,
	linkToValueReturned,

	constRefferenceToValueReturned,
	valueWasRead,

	refferenceToTheItemOfTheArrayValueReturned
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
	virtual void wrapperConstructed() {};
	virtual void uniCallback(SetOfUniWrapperCallbacks setOfCallbacks, T value) {};
	virtual void valueConstructed(T value) {};
	virtual void valueAssigned(T value) {};
	virtual void valueModified(T value) {};
	virtual void valueWritten(T value) {}; // Any change
	virtual void valueDestructed(T value) {};
	virtual void wrapperDestructed() {};
	virtual void classConvertedToValueType(T value) {}; // Read
	virtual void refferenceToValueReturned(T value) {};
	virtual void pointerToValueReturned(T value) {};
	virtual void linkToValueReturned(T value) {}; // Any type of links was returned from the class
	virtual void constRefferenceToValueReturned(T value) {};
	virtual void valueWasRead(T value) {};
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
	{};
	~UniWrapperCalbacksHolder() {};

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