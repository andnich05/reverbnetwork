#ifndef VALUECONVERSION_H
#define VALUECONVERSION_H

class ValueConversion
{
public:
	ValueConversion();
	~ValueConversion();

	static void setSampleRate(const unsigned long s);

	static double normToValueMixerInputSelect(const double& normValue);
	static double valueToNormMixerInputSelect(const double& value);
	static double normToValueFilterTypeSelect(const double& normValue);
	static double valueToNormFilterTypeSelect(const double& value);
	static double normToValueCenterFreq(const double& normValue);
	static double valueToNormCenterFreq(const double& value);
	static double normToValueQFactor(const double& normValue);
	static double valueToNormQFactor(const double& value);
	static double normToValueEqGain(const double& normValue);
	static double valueToNormEqGain(const double& value);
	static double normToValueDelay(const double& normValue);
	static double valueToNormDelay(const double& value);
	static double normToValueDecay(const double& normValue);
	static double valueToNormDecay(const double& value);
	static double normToValueGain(const double& normValue);
	static double valueToNormGain(const double& value);

	static double linearToLog(const double& linearValue);
	static double logToLinear(const double& logValue);

	// This conversion is performed when getValue() is called on a textEdit; converts to float with atof()
	static bool textEditStringToValueConversionCenterFreq(const char* txt, float& result, void* userData);
	// This conversion is performed when the user does some input in the textEdit; converts the string to float with sprintf(%f...)
	static bool textEditValueToStringConversionCenterFreq(float value, char utf8String[256], void* userData);
	static bool textEditStringToValueConversionQFactor(const char* txt, float& result, void* userData);
	static bool textEditValueToStringConversionQFactor(float value, char utf8String[256], void* userData);
	static bool textEditStringToValueConversionEqGain(const char* txt, float& result, void* userData);
	static bool textEditValueToStringConversionEqGain(float value, char utf8String[256], void* userData);
	static bool textEditStringToValueConversionDelay(const char* txt, float& result, void* userData);
	static bool textEditValueToStringConversionDelay(float value, char utf8String[256], void* userData);
	static bool textEditStringToValueConversionDecay(const char* txt, float& result, void* userData);
	static bool textEditValueToStringConversionDecay(float value, char utf8String[256], void* userData);
	static bool textEditStringToValueConversionGain(const char* txt, float& result, void* userData);
	static bool textEditValueToStringConversionGain(float value, char utf8String[256], void* userData);

private:
	static double sampleRate;

};

#endif // VALUECONVERSION_H