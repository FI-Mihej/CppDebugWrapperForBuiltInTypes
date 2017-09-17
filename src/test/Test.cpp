// Test.cpp : Defines the entry point for the console application.
//

#include <string>
#include <cassert>
#include <conio.h>
#include <algorithm>

#include "UniWrapperForBuiltInTypes.hpp"

template<typename T, typename PringStringValue = void>
class MyUniWrapperCalbacks: public UniWrapperCalbacks<T>
{
public:
	MyUniWrapperCalbacks(bool pringStringValue = false)
		:pringStringValue(pringStringValue)
	{

	}
	~MyUniWrapperCalbacks() override
	{

	}

	template<typename T, typename std::enable_if_t<std::is_pointer<T>::value, int> = 0>
	void safePrintValue(T value)
	{
		std::cout << static_cast<void*>(value);
	}

	template<typename T, typename = std::enable_if_t<!std::is_pointer<T>::value> >
	void safePrintValue(T value)
	{
		std::cout << value;
	}

	//template<typename T, typename std::enable_if_t<std::is_void<PringStringValue>::value, int> = 0>
	//void printValue(T value)
	//{
	//	safePrintValue(value);
	//}

	//template<typename T, typename = std::enable_if_t<!std::is_void<PringStringValue>::value> >
	//void printValue(T value)
	//{
	//	std::cout << value;
	//}

	// CALLBACKS
	void wrapperConstructed() override
	{
		std::cout << "\t>> Wrapper Constructed;" << std::endl;
	}

	void uniCallback(SetOfUniWrapperCallbacks setOfCallbacks, T value) override
	{
		// Lets assume we are not interested in valueDestructed callback here
		if ((setOfCallbacks.end() == setOfCallbacks.find(UniWrapperCallbacks::valueDestructed))
			|| ((setOfCallbacks.end() != setOfCallbacks.find(UniWrapperCallbacks::valueDestructed)) 
				&& (setOfCallbacks.size() > 1)))
		{
			std::cout << "\t\t>> ";
			for (auto callbackId : setOfCallbacks) {
				if (UniWrapperCallbacks::valueDestructed != callbackId)
				{
					std::cout << callbackNames(callbackId) << " [ ";
					if (pringStringValue)
						std::cout << value;
					else
						safePrintValue(value);
					//safePrintValue(value);
					safePrintValue(value);
					std::cout<< " ]; ";
				}
			}
			std::cout << std::endl;
		}
	}

	void valueDestructed(T value) override
	{
		std::cout << "\t>> Destructed" << " [ " << value << " ];" << std::endl;
	}

	void wrapperDestructed() override
	{
		std::cout << "\t\t>> Wrapper Destructed." << std::endl;
	}

protected:
	bool pringStringValue = false;
};

//==================================================================================
template<typename TestType, typename OutStreamType, typename SimpleChar, typename SimpleString, typename SmartString>
void testString(OutStreamType &outStream, SimpleString originalString, SimpleChar constChar, int offset)
{
	SimpleChar originalConstChar = constChar;
	int originalOffset = offset;

	using OriginalType = SimpleString;
	MyUniWrapperCalbacks<OriginalType> myCallbacks(true);
	CallbacksChanger<OriginalType> callbacksChanger(&myCallbacks);
	Singleton<UniWrapperCalbacksHolder<OriginalType> >().setCallbacks(myCallbacks);
	
	SmartString originalSmartString = originalString;
	TestType testString = originalString;
	assert(originalSmartString == SmartString(testString));

	outStream << ">> String Value Before: Original [" << originalString << "]" << std::endl;

	SimpleString secondStr0 = originalString + offset;
	SimpleString secondStr1 = testString + offset;
	SimpleString secondStr2 = offset + testString;
	assert(secondStr0 == secondStr1);
	assert(secondStr1 == secondStr2);
	assert(testString[offset] == originalString[offset]);
	SimpleString firstStr = secondStr2 - offset;
	assert(originalString == testString);
	assert(firstStr == testString);
	SimpleChar testChar = testString[offset];
	testString[offset] = constChar;
	testString[offset] = testChar;
	assert(SmartString(secondStr0) == SmartString(secondStr2));
	assert(originalSmartString == SmartString(originalString));
	assert(originalSmartString == SmartString(testString));

	outStream << ">> String Value After: ";
	outStream << "Original [ " << originalString << " ], ";
	outStream << "Test [ " << testString << " ], ";
	outStream << "Offset [ " << secondStr1 << " ], ";
	outStream << "Offset Char [ " << testString[offset] << " ]";
	outStream << std::endl;

	assert(originalConstChar == constChar);
	assert(offset == originalOffset);
}


template<typename TestType, typename OriginalType, typename std::enable_if_t<std::is_pointer<OriginalType>::value, int> = 0>
TestType subtestPointer(TestType testValue, int &constValue)
{
	using BaseOfOriginalType = typename std::remove_pointer<typename OriginalType>::type;
	BaseOfOriginalType k = *testValue;
	OriginalType kp = testValue;
	assert(k == *kp);

	++testValue;
	testValue++;
	--testValue;
	testValue--;

	testValue = testValue - constValue;
	testValue = testValue + constValue;
	testValue = constValue + testValue;
	
	testValue += constValue;
	testValue -= constValue;

	testValue += (unsigned int)constValue;
	testValue -= (unsigned int)constValue;

	return testValue;
}

template<typename TestType, typename OriginalType, typename = std::enable_if_t<!std::is_pointer<OriginalType>::value> >
TestType subtestPointer(TestType testValue, int &constValue)
{
	return testValue;
}

template<typename TestType, typename OriginalType, typename std::enable_if_t<std::is_same<OriginalType, char*>::value || std::is_same<OriginalType, wchar_t*>::value, int> = 0>
TestType subtestString(TestType testValue, int &constValue)
{
	using BaseOfOriginalType = typename std::remove_pointer<typename OriginalType>::type;
	if (std::is_same<OriginalType, wchar_t*>::value)
	{
		wchar_t readOnlyString[] = L"Hello Word";
		wchar_t readWriteString[80];
		wcsncpy(readWriteString, readOnlyString, sizeof(readOnlyString));
		testString<UniWrapperPointer<wchar_t*>, std::wostream, wchar_t, wchar_t*, std::wstring>(std::wcout, readWriteString, L'k', 6);
	}
	else
	{
		char readOnlyString[] = "Hello Word";
		char readWriteString[40];
		strncpy(readWriteString, readOnlyString, sizeof(readOnlyString));
		testString<UniWrapperPointer<char*>, std::ostream, char, char*, std::string>(std::cout, readWriteString, 'k', 6);
	}

	return testValue;
}

template<typename TestType, typename OriginalType, typename = std::enable_if_t<!(std::is_same<OriginalType, char*>::value || std::is_same<OriginalType, wchar_t*>::value)> >
TestType subtestString(TestType testValue, int &constValue)
{
	return testValue;
}

template<typename TestType, typename OriginalType, typename std::enable_if_t<std::is_integral<OriginalType>::value, int> = 0>
TestType subtestIntegral(TestType testValue, int &constValue)
{
	TestType testConstValue = constValue;

	{
		TestType buff = testValue;
		OriginalType buffOriginal = testValue;
		buff %= constValue;
		buffOriginal %= constValue;
		assert(buff == buffOriginal);

		TestType tt = testValue;
		tt %= testConstValue;
		assert(tt == buffOriginal);
	}

	{
		TestType buff = testValue;
		OriginalType buffOriginal = testValue;
		buff = buff % constValue;
		buffOriginal = buffOriginal % constValue;
		assert(buff == buffOriginal);

		TestType tt = testValue;
		tt = tt % testConstValue;
		assert(tt == buffOriginal);
	}

	{
		TestType buff = testValue;
		OriginalType buffOriginal = testValue;
		buff = constValue % buff;
		buffOriginal = constValue % buffOriginal;
		assert(buff == buffOriginal);

		TestType tt = testValue;
		tt = testConstValue % tt;
		assert(tt == buffOriginal);
	}

	assert(testConstValue == constValue);

	return testValue;
}

template<typename TestType, typename OriginalType, typename = std::enable_if_t<!std::is_integral<OriginalType>::value> >
TestType subtestIntegral(TestType testValue, int &constValue)
{
	return testValue;
}

template<typename TestType, typename OriginalType, typename std::enable_if_t<std::is_arithmetic<OriginalType>::value, int> = 0>
TestType subtestArithmetic(TestType testValue, OriginalType initial, int &constValue)
{
	TestType testConstValue = constValue;

	++testValue;
	testValue++;
	--testValue;
	testValue--;

	testValue = testValue - constValue;
	testValue = testValue + constValue;
	{
		int buffConstValue = constValue;
		if (buffConstValue < testValue)
			buffConstValue += testValue;
		testValue = buffConstValue - testValue;
		testValue = buffConstValue + testValue;
	}
	testValue = testValue - testConstValue;
	testValue = testValue + testConstValue;

	testValue += constValue;
	testValue -= constValue;
	{
		TestType buff = testValue;
		testValue += buff;
		testValue -= buff;

		OriginalType buffOriginal = buff;
		buff += buff;
		buffOriginal += buffOriginal;
		assert(buff == buffOriginal);

		buff -= buff;
		buffOriginal -= buffOriginal;
		assert(buff == buffOriginal);
	}

	{
		TestType buff = testValue;
		testValue = constValue * buff;
		testValue /= constValue;
		testValue = testConstValue * buff;
		testValue /= testConstValue;
		if (!buff)
			buff = initial / 2;
		int buffConstValue = constValue;
		if (buffConstValue < buff)
			buffConstValue *= buff;
		TestType tt1 = buffConstValue / buff;
		TestType bcv2 = constValue;
		if (bcv2 < buff)
			bcv2 *= buff;
		TestType tt2 = buffConstValue / buff;
		testValue += tt1 + tt2;
	}
	testValue = testValue / constValue;
	testValue = testValue * constValue;
	testValue = testValue / testConstValue;
	testValue = testValue * testConstValue;

	testValue *= constValue;
	testValue /= constValue;
	{
		TestType buff = testValue;
		testValue *= buff;
		if (!buff)
			buff = initial / 3;
		testValue /= buff;

		OriginalType buffOriginal = buff;
		buff *= buff;
		buffOriginal *= buffOriginal;
		assert(buff == buffOriginal);

		if (!buff)
			buffOriginal = buff = initial / 4;
		buff /= buff;
		buffOriginal /= buffOriginal;
		assert(buff == buffOriginal);
	}

	assert(testConstValue == constValue);

	return testValue;
}

template<typename TestType, typename OriginalType, typename = std::enable_if_t<!std::is_arithmetic<OriginalType>::value> >
TestType subtestArithmetic(TestType testValue, OriginalType initial, int &constValue)
{
	return testValue;
}

template<typename TestType, typename OriginalType, typename std::enable_if_t<std::is_pointer<OriginalType>::value, int> = 0>
TestType subtestCout(TestType testValue, int &constValue)
{
	TestType buff = testValue;
	std::cout << ">> Result Value: [" << testValue << "] [" << *testValue << "]" << std::endl;
	assert(*buff == *testValue);
	assert(buff == testValue);

	return testValue;
}

template<typename TestType, typename OriginalType, typename = std::enable_if_t<!std::is_pointer<OriginalType>::value> >
TestType subtestCout(TestType testValue, int &constValue)
{
	TestType buff = testValue;
	std::cout << ">> Result Value: [" << testValue << "]" << std::endl;
	assert(buff == testValue);

	return testValue;
}

template<typename TestType, typename OriginalType>
TestType universalTestComputerWithConst(OriginalType initial, int originalConstValue)
{
	std::string testTypeName = typeid(TestType).name();
	std::string originalTypeName = typeid(OriginalType).name();

	std::cout << ">> ====================================" << std::endl;
	std::cout << ">> START Test [" << testTypeName << "]" << std::endl;

	TestType j = initial;
	TestType i;
	i = initial;
	assert(i == j);
	assert(j == i);

	if (i == j)
		i++;
	assert(j != i);

	i = j;
	assert(i == j);
	assert(j == i);

	std::cout << ">> Initial Value: [" << i << "]" << std::endl;

	int constValue = originalConstValue;

	{
		using OriginalTypePointer = typename std::add_pointer<typename OriginalType>::type;

		OriginalTypePointer k = &i;
		assert(*k == i);
	}

	i = subtestString<TestType, OriginalType>(i, constValue);

	i = subtestPointer<TestType, OriginalType>(i, constValue);

	i = subtestIntegral<TestType, OriginalType>(i, constValue);

	i = subtestArithmetic<TestType, OriginalType>(i, initial, constValue);

	assert(constValue == originalConstValue);

	i = subtestCout<TestType, OriginalType>(i, constValue);

	std::cout << ">> END Test [" << testTypeName << "]" << std::endl;
	std::cout << ">> ====================================" << std::endl;

	return i;

//return TestType(initial);
}

template<typename TestType, typename OriginalType>
void universalTest(OriginalType initial, std::string comment)
{
	//using TestType = typename UniWrapperUniversal<OriginalType>;
	//using TestType = typename UniWrapperIntegral<OriginalType>;
	//using TestType = typename UniWrapperPointer<OriginalType>;
	//using TestType = typename UniWrapperFundamental<OriginalType>;

	std::string testTypeName = typeid(TestType).name();
	std::string testTitle = "<< Test - [" + testTypeName + "] - " + comment + " >>";
	std::transform(testTitle.begin(), testTitle.end(), testTitle.begin(), ::toupper);

	std::cout << std::endl << std::endl << std::endl << testTitle << std::endl << std::endl;

	const int constValue = 7;

	OriginalType originalResult = universalTestComputerWithConst<OriginalType, OriginalType>(initial, constValue);
	std::cout << std::endl;
	TestType testResult = universalTestComputerWithConst<TestType, OriginalType>(initial, constValue);

	assert(testResult == originalResult);
	assert(originalResult == testResult);
}

//==================================================================================

int main()
{
	{
		universalTest<UniWrapperIntegral<size_t>, size_t>(420000, "without singleton default callbacks change");
	}

	{
		using OriginalType = int;
		MyUniWrapperCalbacks<OriginalType> myCallbacks;
		Singleton<UniWrapperCalbacksHolder<OriginalType> >().setCallbacks(myCallbacks);

		universalTest<UniWrapperIntegral<OriginalType>, OriginalType>(42, "");
		Singleton<UniWrapperCalbacksHolder<OriginalType> >().clearCallbacks();
	}

	{
		using OriginalType = float;
		MyUniWrapperCalbacks<OriginalType> myCallbacks;
		CallbacksChanger<OriginalType> callbacksChanger(&myCallbacks);

		universalTest<UniWrapperFundamental<OriginalType>, OriginalType>(42.0, "");
	}

	{
		using OriginalType = double;
		MyUniWrapperCalbacks<OriginalType> myCallbacks;
		CallbacksChanger<OriginalType> callbacksChanger(&myCallbacks);

		universalTest<UniWrapperFundamental<OriginalType>, OriginalType>(42.0, "");
	}

	{
		using OriginalType = int*;
		CallbacksChanger<OriginalType> callbacksChanger(new MyUniWrapperCalbacks<OriginalType>(), true);

		int i = 42;
		universalTest<UniWrapperPointer<OriginalType>, OriginalType>(&i, "");
	}

	{
		using OriginalType = char*;
		MyUniWrapperCalbacks<OriginalType> myCallbacks;
		CallbacksChanger<OriginalType> callbacksChanger(&myCallbacks);

		char readOnlyString[] = "Hello Word";
		char readWriteString[40];
		strncpy(readWriteString, readOnlyString, sizeof(readOnlyString));
		universalTest<UniWrapperPointer<OriginalType>, OriginalType>(readWriteString, "");
	}

	{
		using OriginalType = wchar_t*;
		MyUniWrapperCalbacks<OriginalType> myCallbacks;
		CallbacksChanger<OriginalType> callbacksChanger(&myCallbacks);

		wchar_t readOnlyString[] = L"Hello Word";
		wchar_t readWriteString[80];
		wcsncpy(readWriteString, readOnlyString, sizeof(readOnlyString));
		universalTest<UniWrapperPointer<OriginalType>, OriginalType>(readWriteString, "");
	}




	//{
	//	std::cout << std::endl << std::endl << std::endl << "TEST - SIZE_T - without singleton default callbacks change" << std::endl << std::endl;

	//	UniWrapperIntegral<rsize_t> i = 4;
	//	++i;
	//	i++;
	//	--i;
	//	i--;
	//	int k = 4 - i;
	//	int kk = i - 4;
	//	size_t* kkk = &i;
	//	k = 4 % i;
	//}

	//{
	//	std::cout << std::endl << std::endl << std::endl << "TEST - INT" << std::endl << std::endl;

	//	MyUniWrapperCalbacks<int> myCallbacks;
	//	Singleton<UniWrapperCalbacksHolder<int> >().setCallbacks(myCallbacks);

	//	UniWrapperIntegral<int> i = 4;
	//	++i;
	//	i++;
	//	--i;
	//	i--;
	//	int k = 4 - i;
	//	int kk = i - 4;
	//	int* kkk = &i;
	//	k = 4 % i;
	//}

	//{
	//	std::cout << std::endl << std::endl << std::endl << "TEST - INT*" << std::endl << std::endl;

	//	MyUniWrapperCalbacks<int*> myCallbacks;
	//	Singleton<UniWrapperCalbacksHolder<int*> >().setCallbacks(myCallbacks);

	//	int a = 10;
	//	UniWrapperPointer<int*> i = &a;
	//	++i;
	//	i++;
	//	--i;
	//	i--;
	//	int *k = 4 + i;
	//	i = UniWrapperPointer<int*>(k);
	//	int *kk = i - 4;
	//	int** kkk = &i;
	//	//i = i + kk;  // '+': cannot add two pointers
	//	//int p = 4 % i;  // Unsupported by C/C++ design so not implemented in the Wrapper
	//	//int p = k % kk;  // Unsupported by C/C++ design so not implemented in the Wrapper
	//	int p = *i;
	//	int *pp = i;
	//	//kk = kk + pp;  // '+': cannot add two pointers
	//	std::cout << p << "; " << *pp << std::endl;
	//}

	//{
	//	std::cout << std::endl << std::endl << std::endl << "TEST - FLOAT" << std::endl << std::endl;

	//	MyUniWrapperCalbacks<float> myCallbacks;
	//	Singleton<UniWrapperCalbacksHolder<float> >().setCallbacks(myCallbacks);

	//	UniWrapperFundamental<float> i = 4.0;
	//	++i;
	//	i++;
	//	--i;
	//	i--;
	//	float k = 4.0 + i;
	//	float kk = i + 4.0;
	//	float* kkk = &i;
	//	//k = i % UniWrapperFundamental<float>(16);  // Unsupported by C/C++ design so not implemented in the Wrapper
	//}

	//{
	//	std::cout << std::endl << std::endl << std::endl << "TEST - DOUBLE - without callbacks" << std::endl << std::endl;

	//	UniWrapperCalbacks<double> defaultCallbacks;
	//	Singleton<UniWrapperCalbacksHolder<double> >().setCallbacks(defaultCallbacks);

	//	UniWrapperFundamental<double> i = 4.0;
	//	++i;
	//	i++;
	//	--i;
	//	i--;
	//	float k = 4.0 + i;
	//	float kk = i + 4.0;
	//	double* kkk = &i;
	//	//k = i % UniWrapperFundamental<float>(16);  // Unsupported by C/C++ design so not implemented in the Wrapper
	//}

	//{
	//	std::cout << std::endl << std::endl << std::endl << "TEST - CHAR*" << std::endl << std::endl;

	//	MyUniWrapperCalbacks<char*> myCallbacks;
	//	Singleton<UniWrapperCalbacksHolder<char*> >().setCallbacks(myCallbacks);

	//	char* originalWord = "Hello Word";
	//	std::cout << originalWord << std::endl;
	//	UniWrapperPointer<char*> word = originalWord;
	//	assert(originalWord == word);
	//	std::cout << originalWord << std::endl;

	//	char* secondWord1 = word + 6;
	//	char* secondWord2 = 6 + word;
	//	assert(secondWord1 == secondWord2);
	//	char* firstWord = secondWord2 - 6;
	//	assert(originalWord == word);
	//	assert(firstWord == word);

	//	std::cout << originalWord << std::endl;
	//	std::cout << word << std::endl;
	//	std::cout << firstWord << std::endl;
	//	std::cout << secondWord1 << std::endl;
	//	std::cout << secondWord2 << std::endl;

	//	char firstCharOfTheSecondWord = word[6];
	//	std::cout << firstCharOfTheSecondWord << std::endl;
	//}

	std::cout << std::endl;
	std::cout << ">> ========================================================================" << std::endl;
	std::cout << ">> ========================================================================" << std::endl;
	std::cout << ">> Press Enter to Exit" << std::endl;
	std::cin.get();

	return 0;
}

