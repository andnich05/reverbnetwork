#include "ValueConversion.h"
#include "ReverbNetworkDefines.h"
#include <cstdlib>
#include <string>
#include "ReverbNetworkEnums.h"

double ValueConversion::sampleRate = 0.0;

ValueConversion::ValueConversion() {
	
}

ValueConversion::~ValueConversion() {
}

void ValueConversion::setSampleRate(const unsigned long s) {
	sampleRate = s;
}

double ValueConversion::normToValueMixerInputSelect(const double& normValue) {
	return normValue * ((float)MAXMODULENUMBER + (float)MAXVSTINPUTS);
}

double ValueConversion::valueToNormMixerInputSelect(const double& value) {
	return value * (1 / ((float)MAXMODULENUMBER + (float)MAXVSTINPUTS));
}

double ValueConversion::normToValueFilterTypeSelect(const double& normValue) {
	return normValue * ((float)(FilterType::numberOfFilterTypes - 1));	
}

double ValueConversion::valueToNormFilterTypeSelect(const double& value) {
	return value * (1 / ((float)(FilterType::numberOfFilterTypes - 1)));
}

double ValueConversion::normToValueCenterFreq(const double& normValue) {
	return (MINEQCENTERFREQ + (sampleRate / 2 - MINEQCENTERFREQ) * normValue);
}

double ValueConversion::valueToNormCenterFreq(const double& value) {
	return ((value - MINEQCENTERFREQ) / (sampleRate / 2 - MINEQCENTERFREQ));
}

double ValueConversion::normToValueQFactor(const double& normValue) {
	return (MINEQQFACTOR + (MAXEQQFACTOR - MINEQQFACTOR) * normValue);
}

double ValueConversion::valueToNormQFactor(const double& value) {
	return ((value - MINEQQFACTOR) / (MAXEQQFACTOR - MINEQQFACTOR));
}

double ValueConversion::normToValueEqGain(const double& normValue) {
	return (MINEQGAINDB + (MAXEQGAINDB - MINEQGAINDB) * normValue);
}

double ValueConversion::valueToNormEqGain(const double& value) {
	return ((value - MINEQGAINDB) / (MAXEQGAINDB - MINEQGAINDB));
}

double ValueConversion::normToValueDelay(const double& normValue) {
	return (normValue * MAXDELAY);
}

double ValueConversion::valueToNormDelay(const double& value) {
	return (value / MAXDELAY);
}

double ValueConversion::normToValueDecay(const double& normValue) {
	return (normValue * MAXDECAY);
}

double ValueConversion::valueToNormDecay(const double& value) {
	return (value / MAXDECAY);
}

double ValueConversion::normToValueGain(const double& normValue) {
	return (MINOUTPUTGAINDB + (MAXOUTPUTGAINDB - MINOUTPUTGAINDB) * normValue);
}

double ValueConversion::valueToNormGain(const double& value) {
	return ((value - MINOUTPUTGAINDB) / (MAXOUTPUTGAINDB - MINOUTPUTGAINDB));
}

double ValueConversion::linearToLog(const double& linearValue) {
	return 20 * log10(linearValue);
}

double ValueConversion::logToLinear(const double& logValue) {
	return pow(10, (logValue / 20));
}

bool ValueConversion::textEditStringToValueConversionCenterFreq(const char* txt, float& result, void* userData) {
	result = (atof(txt));
	return true;
}

bool ValueConversion::textEditValueToStringConversionCenterFreq(float value, char utf8String[256], void* userData) {
	sprintf(utf8String, "%1.0f", (value));
	return true;
}

bool ValueConversion::textEditStringToValueConversionQFactor(const char* txt, float& result, void* userData) {
	result = atof(txt);
	return true;
}

bool ValueConversion::textEditValueToStringConversionQFactor(float value, char utf8String[256], void* userData) {
	sprintf(utf8String, "%1.2f", value);
	return true;
}

bool ValueConversion::textEditStringToValueConversionEqGain(const char* txt, float& result, void* userData) {
	result = atof(txt);
	return true;
}

bool ValueConversion::textEditValueToStringConversionEqGain(float value, char utf8String[256], void* userData) {
	sprintf(utf8String, "%1.2f", value);
	return true;
}

bool ValueConversion::textEditStringToValueConversionDelay(const char* txt, float& result, void* userData) {
	result = atof(txt);
	return true;
}

bool ValueConversion::textEditValueToStringConversionDelay(float value, char utf8String[256], void* userData) {
	sprintf(utf8String, "%1.2f", value);
	return true;
}

bool ValueConversion::textEditStringToValueConversionDecay(const char* txt, float& result, void* userData) {
	result = atof(txt);
	return true;
}

bool ValueConversion::textEditValueToStringConversionDecay(float value, char utf8String[256], void* userData) {
	sprintf(utf8String, "%1.2f", value);
	return true;
}

bool ValueConversion::textEditStringToValueConversionGain(const char* txt, float& result, void* userData) {
	result = atof(txt);
	return true;
}

bool ValueConversion::textEditValueToStringConversionGain(float value, char utf8String[256], void* userData) {
	sprintf(utf8String, "%1.2f", value);
	return true;
}