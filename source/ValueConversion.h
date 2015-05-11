#ifndef VALUECONVERSION_H
#define VALUECONVERSION_H

class ValueConversion
{
public:
	ValueConversion();
	~ValueConversion();


	static double normToValueDelay(const double& normDelay, const unsigned int& sampleRate);
	static double normToValueDecay(const double& normDecay);
	// This conversion is performed when getValue() is called on a textEdit; converts to float with atof()
	static bool textEditStringToValueConversion(const char* txt, float& result, void* userData);
	// This conversion is performed when the user does some input in the textEdit; converts the string to float with sprintf(%f...)
	static bool textEditValueToStringConversion(float value, char utf8String[256], void* userData);


};

#endif // VALUECONVERSION_H