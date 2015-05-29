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
	return normValue * ((double)MAXMODULENUMBER + (double)MAXVSTINPUTS);
}

double ValueConversion::valueToNormMixerInputSelect(const double& value) {
	return value * (1 / ((double)MAXMODULENUMBER + (double)MAXVSTINPUTS));
}

double ValueConversion::normToValueInputGain(const double& normValue) {
	return (MIN_MIXERGAIN + (MAX_MIXERGAIN - MIN_MIXERGAIN) * normValue);
}

double ValueConversion::valueToNormInputGain(const double& value) {
	return ((value - MIN_MIXERGAIN) / (MAX_MIXERGAIN - MIN_MIXERGAIN));
}

double ValueConversion::normToValueFilterTypeSelect(const double& normValue) {
	return normValue * ((double)(FilterType::numberOfFilterTypes - 1));
}

double ValueConversion::valueToNormFilterTypeSelect(const double& value) {
	return value * (1 / ((double)(FilterType::numberOfFilterTypes - 1)));
}

double ValueConversion::normToValueQuantization(const double& normValue) {
	return (MIN_QUANTIZERBITDEPTH + (MAX_QUANTIZERBITDEPTH - MIN_QUANTIZERBITDEPTH) * normValue);
}

double ValueConversion::valueToNormQuantization(const double& value) {
	return ((value - MIN_QUANTIZERBITDEPTH) / (MAX_QUANTIZERBITDEPTH - MIN_QUANTIZERBITDEPTH));
}

double ValueConversion::normToValueCenterFreq(const double& normValue) {
	return (MIN_EQCENTERFREQ + (MAX_EQCENTERFREQ - MIN_EQCENTERFREQ) * normValue);
}

double ValueConversion::valueToNormCenterFreq(const double& value) {
	return ((value - MIN_EQCENTERFREQ) / (MAX_EQCENTERFREQ - MIN_EQCENTERFREQ));
}

double ValueConversion::normToValueQFactor(const double& normValue) {
	return (MIN_EQQFACTOR + (MAX_EQQFACTOR - MIN_EQQFACTOR) * normValue);
}

double ValueConversion::valueToNormQFactor(const double& value) {
	return ((value - MIN_EQQFACTOR) / (MAX_EQQFACTOR - MIN_EQQFACTOR));
}

double ValueConversion::normToValueEqGain(const double& normValue) {
	return (MIN_EQGAIN + (MAX_EQGAIN - MIN_EQGAIN) * normValue);
}

double ValueConversion::valueToNormEqGain(const double& value) {
	return ((value - MIN_EQGAIN) / (MAX_EQGAIN - MIN_EQGAIN));
}

double ValueConversion::normToValueDelay(const double& normValue) {
	return (MIN_ALLPASSDELAY + (MAX_ALLPASSDELAY - MIN_ALLPASSDELAY) * normValue);
}

double ValueConversion::valueToNormDelay(const double& value) {
	return ((value - MIN_ALLPASSDELAY) / (MAX_ALLPASSDELAY - MIN_ALLPASSDELAY));
}

double ValueConversion::delayMillisecondsToSamples(const double& delayMilliseconds) {
	return delayMilliseconds * sampleRate / 1000;
}

double ValueConversion::delaySamplesToMilliseconds(const double& delaySamples) {
	return delaySamples / sampleRate * 1000;
}

double ValueConversion::normToValueDecay(const double& normValue) {
	return (MIN_ALLPASSDECAY + (MAX_ALLPASSDECAY - MIN_ALLPASSDECAY) * normValue);
}

double ValueConversion::valueToNormDecay(const double& value) {
	return ((value - MIN_ALLPASSDECAY) / (MAX_ALLPASSDECAY - MIN_ALLPASSDECAY));
}

double ValueConversion::normToValueOutputGain(const double& normValue) {
	return (MIN_OUTPUTGAIN + (MAX_OUTPUTGAIN - MIN_OUTPUTGAIN) * normValue);
}

double ValueConversion::valueToNormOutputGain(const double& value) {
	return ((value - MIN_OUTPUTGAIN) / (MAX_OUTPUTGAIN - MIN_OUTPUTGAIN));
}

double ValueConversion::linearToLog(const double& linearValue) {
	if (linearValue > 0.0) {
		return 20 * log10(linearValue);
	}
	else {
		return 0.0;
	}
}

double ValueConversion::logToLinear(const double& logValue) {
	return pow(10, (logValue / 20));
}

bool ValueConversion::textEditStringToValueConversion(const char* txt, float& result, void* userData) {
	result = (atof(txt));
	return true;
}

bool ValueConversion::textEditValueToStringConversion(float value, char utf8String[256], void* userData) {
	if (userData == nullptr) {
		sprintf(utf8String, "%1.2f", value);
		return true;
	}
	int* precision = (int*)userData;
	/*
	if (*precision == 0) {
		sprintf(utf8String, "%d", (value));
		return true;
	}
	char format[256];
	sprintf(format, "%1.");
	sprintf(format + strlen(format), "%d", *precision);
	sprintf(format + strlen(format), "f");
	sprintf(utf8String, format, (value));*/

	/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", std::to_string(*((int*)(userData))).c_str());
	fprintf(pFile, "y(n): %s\n", std::to_string(888).c_str());
	fclose(pFile);*/

	switch (*precision) {
	case 0:
		sprintf(utf8String, "%1.0f", value);
		break;
	case 1:
		sprintf(utf8String, "%1.1f", value);
		break;
	case 2:
		sprintf(utf8String, "%1.2f", value);
		break;
	case 3:
		sprintf(utf8String, "%1.3f", value);
		break;
	case 4:
		sprintf(utf8String, "%1.4f", value);
		break;
	case 5:
		sprintf(utf8String, "%1.5f", value);
		break;
	default:
		sprintf(utf8String, "%1.2f", value);
		break;
	}
	return true;
}