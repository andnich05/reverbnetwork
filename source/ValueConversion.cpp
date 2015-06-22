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

double ValueConversion::normToPlainMixerInputSelect(const double& normValue) {
	// Round!
	return std::round(normValue * ((double)MAXMODULENUMBER + (double)MAXVSTINPUTS)); // incl. index 0
}

double ValueConversion::plainToNormMixerInputSelect(const double& plainValue) {
	return plainValue * (1 / ((double)MAXMODULENUMBER + (double)MAXVSTINPUTS)); // incl. index 0
}

double ValueConversion::normToPlainInputGain(const double& normValue) {
	return (MAX_MIXERGAIN - MIN_MIXERGAIN) * normValue + MIN_MIXERGAIN;
}

double ValueConversion::plainToNormInputGain(const double& plainValue) {
	return (plainValue - MIN_MIXERGAIN) / (MAX_MIXERGAIN - MIN_MIXERGAIN);
}

double ValueConversion::normToPlainFilterTypeSelect(const double& normValue) {
	return std::round(normValue * ((double)(FilterType::numberOfFilterTypes - 1)));
}

double ValueConversion::plainToNormFilterTypeSelect(const double& plainValue) {
	return plainValue * (1 / ((double)(FilterType::numberOfFilterTypes - 1)));
}

double ValueConversion::normToPlainQuantization(const double& normValue) {
	return std::round(((double)MAX_QUANTIZERBITDEPTH - (double)MIN_QUANTIZERBITDEPTH) * normValue + (double)MIN_QUANTIZERBITDEPTH);
}

double ValueConversion::plainToNormQuantization(const double& plainValue) {
	return ((double)plainValue - (double)MIN_QUANTIZERBITDEPTH) / ((double)MAX_QUANTIZERBITDEPTH - (double)MIN_QUANTIZERBITDEPTH);
}

double ValueConversion::normToPlainCenterFreq(const double& normValue) {
	return (MAX_EQCENTERFREQ - MIN_EQCENTERFREQ) * normValue + MIN_EQCENTERFREQ;
}

double ValueConversion::plainToNormCenterFreq(const double& plainValue) {
	return (plainValue - MIN_EQCENTERFREQ) / (MAX_EQCENTERFREQ - MIN_EQCENTERFREQ);
}

double ValueConversion::normToPlainQFactor(const double& normValue) {
	return (MAX_EQQFACTOR - MIN_EQQFACTOR) * normValue + MIN_EQQFACTOR;
}

double ValueConversion::plainToNormQFactor(const double& plainValue) {
	return (plainValue - MIN_EQQFACTOR) / (MAX_EQQFACTOR - MIN_EQQFACTOR);
}

double ValueConversion::normToPlainEqGain(const double& normValue) {
	return (MAX_EQGAIN - MIN_EQGAIN) * normValue + MIN_EQGAIN;
}

double ValueConversion::plainToNormEqGain(const double& plainValue) {
	return (plainValue - MIN_EQGAIN) / (MAX_EQGAIN - MIN_EQGAIN);
}

double ValueConversion::normToPlainEqCoefficients(const double& normValue) {
	return (MAX_EQCOEFFICIENTS - MIN_EQCOEFFICIENTS) * normValue + MIN_EQCOEFFICIENTS;
}

double ValueConversion::plainToNormEqCoefficients(const double& plainValue) {
	return (plainValue - MIN_EQCOEFFICIENTS) / (MAX_EQCOEFFICIENTS - MIN_EQCOEFFICIENTS);
}

double ValueConversion::normToPlainDelay(const double& normValue) {
	return (MAX_ALLPASSDELAY - MIN_ALLPASSDELAY) * normValue + MIN_ALLPASSDELAY;
}

double ValueConversion::plainToNormDelay(const double& plainValue) {
	return (plainValue - MIN_ALLPASSDELAY) / (MAX_ALLPASSDELAY - MIN_ALLPASSDELAY);
}

double ValueConversion::delayMillisecondsToSamples(const double& delayMilliseconds) {
	return delayMilliseconds * sampleRate / 1000;
}

double ValueConversion::delaySamplesToMilliseconds(const double& delaySamples) {
	return delaySamples / sampleRate * 1000;
}

double ValueConversion::calculateDiffK(const double& delayInMs, const double& decayInMs) {
	if (decayInMs == 0.0) return 0.0;
	if (delayInMs == 0.0) return 1.0;
	return pow(10, (-60 * (delayInMs / decayInMs) / 20));
}

double ValueConversion::diffKToDecay(const double& diffK, const double& delayInMs) {
	return (-60 / (20 * log10(diffK))) * delayInMs;
}

double ValueConversion::normToPlainDecay(const double& normValue) {
	return (MAX_ALLPASSDECAY - MIN_ALLPASSDECAY) * normValue + MIN_ALLPASSDECAY;
}

double ValueConversion::plainToNormDecay(const double& plainValue) {
	return (plainValue - MIN_ALLPASSDECAY) / (MAX_ALLPASSDECAY - MIN_ALLPASSDECAY);
}

double ValueConversion::normToPlainOutputGain(const double& normValue) {
	return (MAX_OUTPUTGAIN - MIN_OUTPUTGAIN) * normValue + MIN_OUTPUTGAIN;
}

double ValueConversion::plainToNormOutputGain(const double& plainValue) {
	return (plainValue - MIN_OUTPUTGAIN) / (MAX_OUTPUTGAIN - MIN_OUTPUTGAIN);
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
	/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", std::to_string(888).c_str());
	fclose(pFile);*/
	result = (float)(atof(txt));
	return true;
}

bool ValueConversion::textEditValueToStringConversion(float value, char utf8String[256], void* userData) {

	if (userData == nullptr) {
		sprintf(utf8String, "%1.2f", value);
		return true;
	}
	valueToStringUserData* v = (valueToStringUserData*)userData;

	switch (v->precision) {
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
	sprintf(utf8String + strlen(utf8String), " ");
	sprintf(utf8String + strlen(utf8String), v->unit.c_str());
	return true;
}
