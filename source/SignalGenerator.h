#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include "ReverbNetworkEnums.h"
#include <vector>

class SignalGenerator {
public:
	SignalGenerator(const SignalGeneratorType& signalType);
	inline void setSignalType(const SignalGeneratorType& signalType) { this->signalType = signalType; }
	void setGain(const double& gainInDB);
	inline void setWidth(const long int& widthInSamples) { this->width = widthInSamples; }
	inline void setAutoFireEnabled(const bool& enabled) { this->autoFireEnabled = enabled; }
	void setAutoTime(const double& autoTimeInSec);
	inline void setFire(const bool& fire) { this->fire = fire; }
	//inline const bool& getFire(const bool& fire) { return this->fire; }
	
	double& generateSample();

private:
	SignalGeneratorType signalType;
	double gainFactor;
	long int width;
	bool autoFireEnabled;
	long int autoTimeInSamples;
	bool fire;

	double sample;

	long unsigned int autoFireCounter;
	long unsigned int fireCounter;
};


#endif // SIGNALGENERATOR_H