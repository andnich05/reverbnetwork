#include "ValueConversion.h"
#include "ReverbNetworkDefines.h"

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