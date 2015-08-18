/*
* SchroederAllpass: Allpass filter based on Schroeder's paper from 1961
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

#ifndef SCHROEDERALLPASS_H
#define SCHROEDERALLPASS_H

#include "ReverbNetworkEnums.h"

class SchroederAllpass
{
public:
	SchroederAllpass(double delaySec, double decaySec);
	~SchroederAllpass();

	// Process input sample by reference => saving speed
	void doProcessing(double& sample);

	// E.g. When plugin is beeing enabled or disabled
	void createBuffers();
	void freeBuffers();

	// Change current sample rate
	inline void setSampleRate(const double& s) { sampleRate = s; freeBuffers(); createBuffers(); }; // Recreate buffers when sample rate changes
	// Set delay time
	void setDelayTimeSec(const double& sec);
	// Set decay time
	void setDecayTimeSec (const double& sec);
	// Set Allpass gain (unused because the gain is set by setting delay and decay)
	//inline void setGain(const double& g) { gain = g; };
	// Set the sign of the gain
	inline void setGainSign(const bool& isNegative) { this->gainSignIsNegative = isNegative; calculateGain(); }

	// ---Modulation setter functions
	// Enable or disable modulation
	void setModulationEnabled(const bool& enabled);
	// Set the signal type to use for modulation
	void setModulationSignalType(const ModulationSignalType& signalType);
	// Set the time range which is modulated (e.g. delay = 20 ms, excursion = 5 ms => Range: from 15 ms to 25 ms)
	void setModulationExcursion(const double& excursion);
	// Set the modulation speed in Hertz (frequency of the modulation signal e.g. sine)
	void setModulationRateMs(const double& rate);

private:
	//double* inputBuffer; // Circular buffer for input samples x(n)
	//double* outputBuffer; // Circular buffer for output samples y(n)
	double* buffer; // Circular buffer, size is the maximum possible delay in samples
	long bufferSize; // Maximum possible delay in samples
	//unsigned long bufferPos; // Combined read/write index for both circular buffers
	long readPointer; // Read index
	long writePointer; // Write index
	double sampleRate; // Used for buffer creation
	double delaySamples; // Delay value in samples
	double fractDelaySamples; // Fractional part of the delay in samples, used for interpolation
	double delayTimeSec; // Delay time in seconds
	double decayTimeSec; // Decay time in seconds
	double gain; // Gain vaulue for processing
	bool gainSignIsNegative; // Gain sign

	// Modulation parameters
	bool modulationEnabled;
	ModulationSignalType modSignalType;
	double modulationExcursion;
	double modulationRate;

	// Temp values for processing
	//double xn; // = sample
	/*double yn;
	double xnD;
	double ynD;*/
	double nodeLeft; // Left node value in the signal flow diagram
	double nodeRight; // Right node value in the signal flow diagram

	double nodeRightMinusOne;
	double nodeRightNewMinusOne;

	long sampleCounter; // Circular counter used for modulation, counts from 1 to sampleRate / modulationRate (one periode)

	void calculateGain(); // Calculate the loop gain

};

#endif // SCHROEDERALLPASS_H