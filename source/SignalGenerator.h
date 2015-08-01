#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include "ReverbNetworkEnums.h"
#include <vector>

class SignalGenerator {
public:
	SignalGenerator(const SignalGeneratorType& signalType);
	
	// Set the signal type (only dirac impulse for now)
	inline void setSignalType(const SignalGeneratorType& signalType) { this->signalType = signalType; }
	// Set the gain of the dirac (from 0 dB to e.g. -60 dB)
	void setGain(const double& gainInDB);
	// Set width of the pulse in samples
	inline void setWidth(const long int& widthInSamples) { this->width = widthInSamples; }
	// Enable or disable the automatic generation
	inline void setAutoFireEnabled(const bool& enabled) { this->autoFireEnabled = enabled; }
	// Set the period of the automatic generation
	void setAutoTime(const double& autoTimeInSec);
	// Generate a single impulse now
	inline void setFire(const bool& fire) { this->fire = fire; }
	
	// Returns a generated sample
	double& generateSample();

private:
	SignalGeneratorType signalType;
	double gainFactor;
	long int width;
	bool autoFireEnabled;
	long int autoTimeInSamples;
	bool fire;

	double sample; // Generated sample

	long unsigned int autoFireCounter;
	long unsigned int fireCounter;
};


#endif // SIGNALGENERATOR_H