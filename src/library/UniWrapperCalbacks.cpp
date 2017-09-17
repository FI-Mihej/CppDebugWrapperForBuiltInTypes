#include "UniWrapperCalbacks.h"

UniWrapperCallbacksNames::UniWrapperCallbacksNames()
{
	std::map<UniWrapperCallbacks, std::string> names = {
		{ wrapperConstructed, "WrapperConstructed" },
		{ valueConstructed, "Constructed" },
		{ valueAssigned, "Assigned" },
		{ valueModified, "Modified" },
		{ valueWritten, "Written" },
		{ valueDestructed, "Destructed" },
		{ wrapperDestructed, "WrapperDestructed" },
		{ classConvertedToValueType, "ClassConvertedToValueType" },
		{ refferenceToValueReturned, "RefferenceToValueReturned" },
		{ pointerToValueReturned, "PointerToValueReturned" },
		{ linkToValueReturned, "LinkToValueReturned" },
		{ constRefferenceToValueReturned, "ConstRefferenceToValueReturned" },
		{ valueWasRead, "Read" },
		{ refferenceToTheItemOfTheArrayValueReturned, "RefferenceToTheItemOfTheArrayValueReturned" }, };

	this->names = names;
}

UniWrapperCallbacksNames::~UniWrapperCallbacksNames()
{

}

const std::string& UniWrapperCallbacksNames::operator()(UniWrapperCallbacks id)
{
	return names[id];
}
