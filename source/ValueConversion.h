#ifndef VALUECONVERSION_H
#define VALUECONVERSION_H

class ValueConversion
{
public:
	ValueConversion();
	~ValueConversion();


	static double normToValueDelay(const double& normDelay, const unsigned int& sampleRate);
	static double normToValueDecay(const double& normDecay);
	


};

#endif // VALUECONVERSION_H