#include "ValueConversion.h"
#include "ReverbNetworkDefines.h"
#include <cstdlib>
#include <string>

ValueConversion::ValueConversion() {
}


ValueConversion::~ValueConversion() {
}

double ValueConversion::normToValueDelay(const double& normDelay, const unsigned int& sampleRate) {
	return (normDelay * MAXDELAY * sampleRate);
}

double ValueConversion::normToValueDecay(const double& normDecay) {
	return (normDecay * MAXDECAY);
}

bool ValueConversion::textEditStringToValueConversion(const char* txt, float& result, void* userData) {
	result = atof(txt);
	return true;
}

bool ValueConversion::textEditValueToStringConversion(float value, char utf8String[256], void* userData) {
	sprintf(utf8String, "%0.2f", value);
	return true;
}