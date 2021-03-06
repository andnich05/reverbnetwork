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

#include "../include/SignalGenerator.h"
#include "../include/ValueConversion.h"
#include "../include/ReverbNetworkDefines.h"

SignalGenerator::SignalGenerator(SignalGeneratorType signalType) 
	: signalType(signalType)
	, gainFactor(ValueConversion::logToLinear(DEF_SIGNALGENERATOR_AMPLITUDE))
	, width((long)DEF_SIGNALGENERATOR_WIDTH)
	, autoFireEnabled(DEF_SIGNALGENERATOR_AUTOFIREENABLED)
	, autoTimeInSamples((long)(DEF_SIGNALGENERATOR_TIME * ValueConversion::getSampleRate()))
	, autoFireCounter(0)
	, fireCounter(0)
{
}

void SignalGenerator::setGain(double gainInDB) {
	this->gainFactor = ValueConversion::logToLinear(gainInDB);
}

void SignalGenerator::setAutoTime(double autoTimeInSec) {
	this->autoTimeInSamples = (long)(std::round(autoTimeInSec * ValueConversion::getSampleRate()));
}

double SignalGenerator::generateSample() {
	double sample = 0.0;

	// Auto fire generation
	if (autoFireEnabled) {
		// Output samples until the pulse width has been reached
		if (autoFireCounter < (unsigned long)width) {
			sample = gainFactor * 1.0;
		}
		++autoFireCounter;
		// Reset the counter when the auto time has been reached
		if (autoFireCounter >= (unsigned long)autoTimeInSamples) {
			autoFireCounter = 0;
		}
	}

	//// Manual fire generation
	//if (fire) {
	//	fireCounter = width;
	//	fire = false;
	//}
	// Decrement the counter until the pulse width is reached
	if (fireCounter > 0) {
		sample = gainFactor * 1.0;
		--fireCounter;
	}
	return sample;
}
