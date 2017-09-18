# CppDebugWrapperForBuiltInTypes
Wrapper class for built in Cpp (C++) types.

Made for debugging purpose: you can find place where some int/float/char* class member was changed to a certain value.

Works with:
* floating point types (float, double, long double), 
* integral types (int, bool, char16_t, wchar_t, long long, etc.)
* pointer types (int*, char*, etc.)

# How to use

## Problem
Lets assume we want to debug some class MyClass:

```cpp
class MyClass
{
    ...
    float myValue;
    ...
};
```

MyClass is widely used through all our code base.
And some part of code assigns wrong value ( **937.2** ) to the MyClass::myValue floating point member.

But:
- We can't simply place a breakpoint: there will be thousands brekpoints.
- We can't simply debug memory change: MyClass is constructed-destructed friequently from many places in the code.

So wat can we do? We can't spend days for this bug!

## Solution

Is simple

1. Create your callback class derived from the UniWrapperCalbacks class:

```cpp
template<typename T>
class MyUniWrapperCalbacks: public UniWrapperCalbacks<T>
{
public:
    MyUniWrapperCalbacks()
    { }

    ~MyUniWrapperCalbacks() override
    { }

    // CALLBACKS
    void valueWritten(T value) override
    {
        if ((value > 937.1) || (value < 937.3>))
        {
            // Place for a breakpoint, loggin, etc.
            std::cout << "\t>> Wrong value: [ " << value << " ]" << std::endl;
        }
    }
}
```

In our example we are interested in single callback. But you can use any combination.

2. Create and register your Callback object for example in your main():

```cpp
#include "UniWrapperForBuiltInTypes.hpp"

...

int main()
{
    MyUniWrapperCalbacks<float> myCallbacks;
    Singleton<UniWrapperCalbacksHolder<float> >().setCallbacks(myCallbacks);
    ...
}
```

3. Replace type of the interested class member or variable with the appropriate type. In our case it will be **UniWrapperFundamental<...>** since we have a floating point class member:

```cpp
class MyClass
{
    ...
    UniWrapperFundamental<float> myValue;
    ...
};
```

4. Rebuild and run your code. 

That's all.

# Build & integration

There is Visual Studio solution in the folder __"./projects/VS 2015"__

Library file (__CppWrapperForBuiltInTypes.lib__) will be placed in the __"./bin"__ subfolders. For example in a __"./bin/x64/Release/"__ subfolder for a x64 Release build.

To ingegrate it into your code include __UniWrapperForBuiltInTypes.hpp__ from the __"./src/library"__ folder.

# Documentation
## Type wrappers
* For **floating point** types (float, double, long double) types use **UniWrapperFundamental<...>** wrapper
* For **integral** types (int, bool, char16_t, wchar_t, long long, etc.) use **UniWrapperIntegral<...>** wrapper
* For **pointer** types (int*, char*, etc.) use **UniWrapperPointer<...>** wrapper

And don't forget to use an appropriate type parameter for a template:
* If you want to replace **char16_t** - use **UniWrapperIntegral< char16_t >** - not **UniWrapperIntegral< int >** or thomething. 
* If you want to replace **long double** - use **UniWrapperFundamental< long double >** 
* etc.

## Callbacks list

```cpp
template<typename T>
class UniWrapperCalbacks
{
public:
...
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
...
};
```

## uniCallback()
Works only for value related callbacks (wrapper related callbacks can't and should't return value).

```cpp
enum UniWrapperCallbacks {
    valueConstructed,  // Default and Copy constructors
    valueAssigned,  // operator=()
    ...
}

typedef std::set<UniWrapperCallbacks> SetOfUniWrapperCallbacks;

void uniCallback(SetOfUniWrapperCallbacks setOfCallbacks, T value);
```

You can get printable name of the each calback using **const std::string& UniWrapperCalbacks<T>::callbackNames(...)** class member:

```cpp
    void uniCallback(SetOfUniWrapperCallbacks setOfCallbacks, T value) override
    {
        for (auto callbackId : setOfCallbacks) {
            std::cout << callbackNames(callbackId) << "; ";
        }
        std::cou << std::endl;
    }
```

## Callback objects registering
### Unregistering
```cpp
    Singleton<UniWrapperCalbacksHolder<...> >().clearCallbacks();
```

### Nested registering
You can do it by your hands. Or with CallbacksChanger class:

```cpp
// By hands:
{
    UniWrapperCalbacks<T>* oldCallbacks = &(Singleton<UniWrapperCalbacksHolder<T> >().getCallbacks());
    MyUniWrapperCalbacks<T> myCallbacks;
    Singleton<UniWrapperCalbacksHolder<T> >().setCallbacks(myCallbacks);

   ...

    Singleton<UniWrapperCalbacksHolder<T> >().setCallbacks(*oldCallbacks);
}
```

```cpp
// With CallbacksChanger class:
{
    MyUniWrapperCalbacks<T> myCallbacks;
    Singleton<UniWrapperCalbacksHolder<T> >().setCallbacks(myCallbacks);
    CallbacksChanger<OriginalType> callbacksChanger(&myCallbacks);

   ...

}
```

# Some Usefull tips
If you want to print (read/write) value strings (when there is "char* value;", "wchar_t* value;", etc.) within your callbacks - do not forget that pointer to a string can be broken: you are debugging some bug after all. And if you will try to "cout << value;" for a broken pointer - your app will crash. So be carefull with this. For example of solution look to the **void MyUniWrapperCalbacks::uniCallback(...)** callback implementation in __"./src/test/Test.cpp"__

# Examples

Look to the __"./src/test/Test.cpp"__

# License

Apache License Version 2.0