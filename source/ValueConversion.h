#ifndef VALUECONVERSION_H
#define VALUECONVERSION_H

class ValueConversion
{
public:
	ValueConversion();
	~ValueConversion();

	static void setSampleRate(const unsigned long s);
	inline static double getSampleRate() { return sampleRate; }

	static double normToPlainMixerInputSelect(const double& normValue);
	static double plainToNormMixerInputSelect(const double& plainValue);
	static double normToPlainInputGain(const double& normValue);
	static double plainToNormInputGain(const double& plainValue);
	static double normToPlainQuantization(const double& normValue);
	static double plainToNormQuantization(const double& plainValue);
	static double normToPlainFilterTypeSelect(const double& normValue);
	static double plainToNormFilterTypeSelect(const double& plainValue);
	static double normToPlainCenterFreq(const double& normValue);
	static double plainToNormCenterFreq(const double& plainValue);
	static double normToPlainQFactor(const double& normValue);
	static double plainToNormQFactor(const double& plainValue);
	static double normToPlainEqGain(const double& normValue);
	static double plainToNormEqGain(const double& plainValue);
	static double normToPlainDelay(const double& normValue);
	static double plainToNormDelay(const double& plainValue);
	static double delayMillisecondsToSamples(const double& delaySeconds);
	static double delaySamplesToMilliseconds(const double& delaySamples);
	static double calculateDiffK(const double& delayInMs, const double& decayInS);
	static double normToPlainDecay(const double& normValue);
	static double plainToNormDecay(const double& plainValue);
	static double normToPlainOutputGain(const double& normValue);
	static double plainToNormOutputGain(const double& plainValue);

	static double linearToLog(const double& linearValue);
	static double logToLinear(const double& logValue);

	// This conversion is performed when getValue() is called on a textEdit; converts to float with atof()
	static bool textEditStringToValueConversion(const char* txt, float& result, void* userData);
	// This conversion is performed when the user does some input in the textEdit; converts the string to float with sprintf(%f...)
	// userData contains the precision for the value to be displayed
	static bool textEditValueToStringConversion(float value, char utf8String[256], void* userData);

private:
	static double sampleRate;

};

#endif // VALUECONVERSION_H