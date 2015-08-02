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

#include "SchroederAllpass.h"
#include "ReverbNetworkDefines.h"
#include <cmath>
#include <string>

double yaOld = 0;

SchroederAllpass::SchroederAllpass(double delaySec, double decaySec)
	: buffer(nullptr)
	, readPointer(0)
	, writePointer(0)
	, sampleRate(192000.0) // Set the maximum possible sample rate (is being changed as soon as the plugin gets the right sample rate from the host)
	, delaySamples(sampleRate * delaySec)
	, delayTimeSec(delaySec)
	, decayTimeSec(decaySec)
{
	/*yn = 0;
	xnD = 0;
	ynD = 0;*/

	modulationEnabled = DEF_ALLPASSMODENABLED;
	modulationExcursion = DEF_ALLPASSMODEXCURSION / 1000.0;
	modulationRate = DEF_ALLPASSMODRATE;

	sampleCounter = 1;
	bufferSize = 0;
	gain = 0.7;
	fractDelaySamples = 0.0;
	gainSignIsNegative = false;
	nodeLeft = 0.0;
	nodeRight = 0.0;
	setDecayTimeSec(decaySec);
	createBuffers();
}

SchroederAllpass::~SchroederAllpass() {
	freeBuffers();
}

void SchroederAllpass::doProcessing(double& sample) {
	// Version 2: Without difference equation, short version of Schroeder's Allpass filter
	// Only half the memory consumption compared to version 1
	// Only the left node is saved in the ring buffer (left node in this picture: https://ccrma.stanford.edu/~jos/Delay/Schroeder_Allpass_Filters.html)
	// When the next sample arrives the left node becomes the right node
	// Source: apdiff by K. M. Indlekofer
	
	//---
	// Wrong signs???
	//// Calculate the current left node value
	//nodeLeft = sample - gain * nodeRight;
	//// Calculate the output value
	//sample = gain * nodeLeft + nodeRight;
	//---

	if (modulationEnabled) {
		if (delaySamples > 0.0) {
			//---Modulation (Source: Pirkle book p238/239 and DAFX book)
			// Only a sine signal for the moment
			double delaySamplesMod = delaySamples + (sampleRate * modulationExcursion) * sin(2 * M_PI * (modulationRate / sampleRate) * sampleCounter);
			// In case the chosen excursion is greater than the allpass delay

			/*FILE* pFile = fopen("E:\\logVst.txt", "a");
			fprintf(pFile, "y(n): %s\n", std::to_string(delaySamplesMod).c_str());
			fclose(pFile);*/

			if (delaySamplesMod < 0) {
				delaySamplesMod = 0;
			}
			// Get the fractional part of the modulated delay
			fractDelaySamples = delaySamplesMod - (long)delaySamplesMod;

			// Set the distance between the read and write pointer
			readPointer = writePointer - (long)delaySamplesMod;
			// If the read pointer is negative, loop it back
			if (readPointer < 0) {
				readPointer += bufferSize;
			}

			// Increment counter
			++sampleCounter;
			if (sampleCounter > sampleRate / modulationRate) {
				sampleCounter = 1; // !!! Must start at 1 !!!
			}
			//---
		}
	}

	// Read the left node value from last time, it is now the right node value
	nodeRight = buffer[readPointer];

	//---Interpolation (Source: Pirkle book p238/239 and DAFX book)
	// Get the !PREVIOUS! value from the buffer
	double nodeRightPrevious = 0.0;
	if (delaySamples >= 1.0) {
		if (readPointer - 1 < 0) {
			nodeRightPrevious = buffer[bufferSize - 1]; // Loop back
		}
		else {
			nodeRightPrevious = buffer[readPointer - 1];
		}
	}
	else {
		// When delay in samples is smaller than 1.0 => read and write pointer are at the same position similar to when the delay is at maximum
		nodeRight = sample;
	}
	// Linear Interpolation
	nodeRight = nodeRightPrevious * fractDelaySamples + nodeRight * (1.0 - fractDelaySamples);
	//---
	
	// Calculate the current left node value
	nodeLeft = sample + gain * nodeRight;
	// Calculate the output value
	sample = -gain * nodeLeft + nodeRight;

	// Save left node value (it will be the right node value next time)
	buffer[writePointer] = nodeLeft;
	// Increment write pointer
	++writePointer;
	// Reset pointer if it exceeds the delay in samples
	if (writePointer >= bufferSize) {
		writePointer = 0;
	}
	// Increment read pointer
	++readPointer;
	// Reset pointer if it exceeds the delay in samples
	if (readPointer >= bufferSize) {
		readPointer = 0;
	}
}

void SchroederAllpass::createBuffers() {
	freeBuffers(); // just in case...

	// Create pre-initialized arrays for maximum delay value
	buffer = new double[(long int)(sampleRate * (MAX_ALLPASSDELAY / 1000.0))]();
	bufferSize = (long int)(sampleRate * (MAX_ALLPASSDELAY / 1000.0));
}

void SchroederAllpass::freeBuffers() {
	if (buffer) {
		delete[] buffer;
		buffer = nullptr; // !!! Otherwise crash when checking if (inputBuffer)
	}
}

void SchroederAllpass::setDelayTimeSec(const double& sec) {
	delayTimeSec = sec; 
	delaySamples = sec * sampleRate; // DelaySamples is a floating point number!

	// For interpolation
	fractDelaySamples = delaySamples - (long)delaySamples;
	// Set the distance between the read and write pointer
	readPointer = writePointer - (long)delaySamples;
	// If the read pointer is negative, loop it back
	if (readPointer < 0) {
		readPointer += bufferSize;
	}

	calculateGain();
}

void SchroederAllpass::setDecayTimeSec(const double& sec) {
	decayTimeSec = sec;
	calculateGain();
}

void SchroederAllpass::calculateGain() {
	double dB = 0.0;
	if (decayTimeSec != 0.0) {
		// The normal calculation (see Schroeder paper for details)
		dB = -60.0 * (delayTimeSec / decayTimeSec);
		gain = pow(10.0, dB / 20.0);
	}
	else {
		// If decay is zero gain should also be zero => Allpass becomes a simple delay module (Low pass)
		gain = 0.0;
	}
	if (gainSignIsNegative) {
		// Switch the sign
		gain = -gain;
	}
}

void SchroederAllpass::setModulationEnabled(const bool& enabled) {
	this->modulationEnabled = enabled;
}

void SchroederAllpass::setModulationExcursion(const double& excursion) {
	this->modulationExcursion = excursion / 1000.0;
}

void SchroederAllpass::setModulationRateMs(const double& rate) {
	this->modulationRate = rate;
}



//--- Backup: First version with difference equation (two buffers and one pointer):
//#include "SchroederAllpass.h"
//#include "ReverbNetworkDefines.h"
//#include <cmath>
//#include <string>
//
//SchroederAllpass::SchroederAllpass(double sampleRate, double delay, double decay)
//	: inputBuffer(nullptr)
//	, outputBuffer(nullptr)
//	, bufferPos(0)
//	, sampleRate(sampleRate)
//	, delaySamples((unsigned long)sampleRate * delay)
//	, delayTime(delay)
//	, decayTime(decay)
//{
//	yn = 0;
//	xnD = 0;
//	ynD = 0;
//	setDecayTime(decay);
//	createBuffers();
//}
//
//SchroederAllpass::~SchroederAllpass() {
//	freeBuffers();
//}
//
//void SchroederAllpass::doProcessing(double& sample) {
//	// Read current sample
//	//xn = sample; // x(n)
//	// Read stored samples from circular buffers
//	xnD = inputBuffer[bufferPos]; // x(n-D)
//	ynD = outputBuffer[bufferPos]; // y(n-D)
//
//	// Difference equation
//	yn = -gain * sample + xnD + gain * ynD;
//	// Normal delay would be IIR: yn = sample - gain * ynD;
//
//	// Store current input sample in circular buffer
//	inputBuffer[bufferPos] = sample;
//
//	// Write output sample
//	sample = yn;
//
//	// Store current output sample in circular buffer
//	outputBuffer[bufferPos] = yn;
//
//	// Increment index of circular buffers
//	++bufferPos;
//
//	// Reset index if it has exceeded the delay in samples
//	if (bufferPos >= delaySamples) {
//		bufferPos = 0;
//	}
//	//FILE* pFile = fopen("E:\\logVst.txt", "a");
//	////fprintf(pFile, "y(n): %s\n", std::to_string(delayTime).c_str());
//	////fprintf(pFile, "y(n): %s\n", std::to_string(decayTime).c_str());
//	//fprintf(pFile, "y(n): %s\n", std::to_string(gain).c_str());
//	//fclose(pFile);
//}
//
//void SchroederAllpass::createBuffers() {
//	freeBuffers(); // just in case...
//
//	// Create pre-initialized arrays for maximum delay value (round up)
//	inputBuffer = new double[(int)(std::ceil(sampleRate * (MAX_ALLPASSDELAY / 1000.0)))]();
//	outputBuffer = new double[(int)(std::ceil(sampleRate * (MAX_ALLPASSDELAY / 1000.0)))]();
//}
//
//void SchroederAllpass::freeBuffers() {
//	bufferPos = 0;
//
//	if (inputBuffer) {
//		delete[] inputBuffer;
//		inputBuffer = nullptr; // !!! Otherwise crash when checking if (inputBuffer)
//	}
//	if (outputBuffer) {
//		delete[] outputBuffer;
//		outputBuffer = nullptr; // !!!
//	}
//}
//
//void SchroederAllpass::setDelayTimeMsec(const double& ms) {
//	delayTime = ms / 1000;
//	delaySamples = (unsigned long)(delayTime * sampleRate);
//	calculateGain();
//}
//
//void SchroederAllpass::setDecayTime(const double& sec) {
//	decayTime = sec;
//	calculateGain();
//}
//
//void SchroederAllpass::calculateGain() {
//	double dB = 0.0;
//	// Prevent division by zero
//	if (decayTime > 0.0) {
//		dB = -60.0 * (delayTime / decayTime);
//		gain = pow(10.0, dB / 20);
//	}
//	else {
//		// If decay time is zero gain should be zero also => samples are simply delayed by the specified delay time
//		gain = 0.0;
//	}
//
//
//	/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
//	fprintf(pFile, "y(n): %s\n", std::to_string(gain).c_str());
//	fclose(pFile);*/
//}