/*
* SignalGenerator: Generates some basic signals
*
* Copyright (C) 2015  Andrej Nichelmann
*                     Klaus Michael Indlekofer
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
	inline void setFire(const bool& fire) { fireCounter = width; }
	
	// Returns a generated sample
	double& generateSample();

private:
	SignalGeneratorType signalType;
	double gainFactor;
	long int width;
	bool autoFireEnabled;
	long int autoTimeInSamples;
	//bool fire;

	double sample; // Generated sample

	long unsigned int autoFireCounter;
	long unsigned int fireCounter;
};


#endif // SIGNALGENERATOR_H