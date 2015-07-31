#include "SchroederAllpass.h"
#include "ReverbNetworkDefines.h"
#include <cmath>
#include <string>

double yaOld = 0;

SchroederAllpass::SchroederAllpass(double delaySec, double decaySec)
	: buffer(nullptr)
	, readPointer(0)
	, writePointer(0)
	, sampleRate(192000.0) // Set the maximum possible sample rate
	, delaySamples(sampleRate * delaySec)
	, delayTimeSec(delaySec)
	, decayTimeSec(decaySec)
{
	/*yn = 0;
	xnD = 0;
	ynD = 0;*/

	modulationEnabled = DEF_ALLPASSMODENABLED;
	modulationExcursion = DEF_ALLPASSMODEXCURSION;
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
		//---Modulation (Source: Pirkle book p238/239 and DAFX book)
		// MAXIMUM von delaySamplesMod im puffer allokieren!!!!!!!!!!!!!!
		// Only a sine signal for the moment
		double delaySamplesMod = 1 + delaySamples + (sampleRate * modulationExcursion) * sin(2 * M_PI * (modulationRate / sampleRate) * sampleCounter);
		// In case the chosen excursion is greater than the allpass delay
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
		if (sampleCounter > sampleRate) {
			sampleCounter = 1; // !!! Must start at 1 !!!
		}
		//---
	}

	// Read the left node value from last time, it is now the right node value
	nodeRight = buffer[readPointer];

	//---Interpolation (Source: Pirkle book p238/239 and DAFX book)
	// Get the !PREVIOUS! value from the buffer
	double nodeRightPrevious = 0.0;
	if (readPointer - 1 < 0) {
		nodeRightPrevious = buffer[bufferSize - 1]; // Loop back
	}
	else {
		nodeRightPrevious = buffer[readPointer - 1];
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
}

void SchroederAllpass::createBuffers() {
	freeBuffers(); // just in case...

	// Create pre-initialized arrays for maximum delay value (round up)
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
	delaySamples = sec * sampleRate;

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
	
	//if (decayTimeSec > 0.0) { // gain has to be positive
	//	dB = -60.0 * (delayTimeSec / decayTimeSec);
	//	gain = pow(10.0, dB / 20);
	//}
	//else if (decayTimeSec < 0.0) { // gain has to be negative => same result but 180� phase shift
	//	dB = -60.0 * (delayTimeSec / -decayTimeSec);
	//	gain = -pow(10.0, dB / 20);
	//}
	//else { // Prevent division by zero
	//	// If decay time is zero gain should be zero also => samples are simply delayed by the specified delay time
	//	gain = 0.0;
	//}

	if (decayTimeSec != 0.0) {
		dB = -60.0 * (delayTimeSec / decayTimeSec);
		gain = pow(10.0, dB / 20.0);
	}
	else {
		gain = 0.0;
	}
	if (gainSignIsNegative) {
		gain = -gain;
	}
}

void SchroederAllpass::setModulationEnabled(const bool& enabled) {
	this->modulationEnabled = enabled;
}

void SchroederAllpass::setModulationExcursion(const double& excursion) {
	this->modulationExcursion = excursion;
}

void SchroederAllpass::setModulationRateMs(const double& rate) {
	this->modulationRate = rate / 1000.0;
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