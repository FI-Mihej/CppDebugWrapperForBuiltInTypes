// Test.cpp : Defines the entry point for the console application.
//

#include <string>
#include <cassert>
#include <conio.h>

#include "UniWrapperForBuiltInTypes.hpp"

template<typename T>
class MyUniWrapperCalbacks: public UniWrapperCalbacks<T>
{
public:
	MyUniWrapperCalbacks()
	{

	}
	~MyUniWrapperCalbacks() override
	{

	}

	// CALLBACKS
	void uniCallback(SetOfUniWrapperCallbacks setOfCallbacks, T value) override
	{
		// Lets assume we are not interested in valueDestructed callback here
		if ((setOfCallbacks.end() == setOfCallbacks.find(UniWrapperCallbacks::valueDestructed))
			|| ((setOfCallbacks.end() != setOfCallbacks.find(UniWrapperCallbacks::valueDestructed)) 
				&& (setOfCallbacks.size() > 1)))
		{
			std::cout << "\t>> ";
			for (auto callbackId : setOfCallbacks) {
				if (UniWrapperCallbacks::valueDestructed != callbackId)
					std::cout << callbackNames(callbackId) << " [ " << value << " ]; ";
			}
			std::cout << std::endl;
		}
	}

	void valueDestructed(T value) override
	{
		std::cout << "\t\t>> Destructed" << " [ " << value << " ]; " << std::endl;
	}
};

//==================================================================================

int main()
{
	{
		std::cout << std::endl << std::endl << std::endl << "TEST - SIZE_T - without singleton default callbacks change" << std::endl << std::endl;

		UniWrapperUniversal<rsize_t> i = 4;
		++i;
		i++;
		--i;
		i--;
		int k = 4 - i;
		int kk = i - 4;
		size_t* kkk = &i;
		k = 4 % i;
	}

	{
		std::cout << std::endl << std::endl << std::endl << "TEST - INT" << std::endl << std::endl;

		MyUniWrapperCalbacks<int> myCallbacks;
		Singleton<UniWrapperCalbacksHolder<int> >().setCallbacks(myCallbacks);

		UniWrapperUniversal<int> i = 4;
		++i;
		i++;
		--i;
		i--;
		int k = 4 - i;
		int kk = i - 4;
		int* kkk = &i;
		k = 4 % i;
	}

	{
		std::cout << std::endl << std::endl << std::endl << "TEST - INT*" << std::endl << std::endl;

		MyUniWrapperCalbacks<int*> myCallbacks;
		Singleton<UniWrapperCalbacksHolder<int*> >().setCallbacks(myCallbacks);

		int a = 10;
		UniWrapperUniversal<int*> i = &a;
		++i;
		i++;
		--i;
		i--;
		int *k = 4 + i;
		int *kk = i - 4;
		int** kkk = &i;
		//int p = 4 % i;  // Unsupported by C/C++ design so not implemented in the Wrapper
		//int p = k % kk;  // Unsupported by C/C++ design so not implemented in the Wrapper
		int p = *i;
		int *pp = i;
		std::cout << p << "; " << *pp << std::endl;
	}

	{
		std::cout << std::endl << std::endl << std::endl << "TEST - FLOAT" << std::endl << std::endl;

		MyUniWrapperCalbacks<float> myCallbacks;
		Singleton<UniWrapperCalbacksHolder<float> >().setCallbacks(myCallbacks);

		UniWrapperUniversal<float> i = 4.0;
		++i;
		i++;
		--i;
		i--;
		float k = 4.0 + i;
		float kk = i + 4.0;
		float* kkk = &i;
		//k = i % UniWrapperUniversal<float>(16);  // Unsupported by C/C++ design so not implemented in the Wrapper
	}

	{
		std::cout << std::endl << std::endl << std::endl << "TEST - DOUBLE - without callbacks" << std::endl << std::endl;

		UniWrapperCalbacks<double> defaultCallbacks;
		Singleton<UniWrapperCalbacksHolder<double> >().setCallbacks(defaultCallbacks);

		UniWrapperUniversal<double> i = 4.0;
		++i;
		i++;
		--i;
		i--;
		float k = 4.0 + i;
		float kk = i + 4.0;
		double* kkk = &i;
		//k = i % UniWrapperUniversal<float>(16);  // Unsupported by C/C++ design so not implemented in the Wrapper
	}

	{
		std::cout << std::endl << std::endl << std::endl << "TEST - CHAR*" << std::endl << std::endl;

		MyUniWrapperCalbacks<char*> myCallbacks;
		Singleton<UniWrapperCalbacksHolder<char*> >().setCallbacks(myCallbacks);

		char* originalWord = "Hello Word";
		std::cout << originalWord << std::endl;
		UniWrapperUniversal<char*> word = originalWord;
		assert(originalWord == word);
		std::cout << originalWord << std::endl;

		char* secondWord1 = word + 6;
		char* secondWord2 = 6 + word;
		assert(secondWord1 == secondWord2);
		char* firstWord = secondWord2 - 6;
		assert(originalWord == word);
		assert(firstWord == word);

		std::cout << originalWord << std::endl;
		std::cout << word << std::endl;
		std::cout << firstWord << std::endl;
		std::cout << secondWord1 << std::endl;
		std::cout << secondWord2 << std::endl;

		char firstCharOfTheSecondWord = word[6];
		std::cout << firstCharOfTheSecondWord << std::endl;
	}

	std::cin.get();

	return 0;
}

