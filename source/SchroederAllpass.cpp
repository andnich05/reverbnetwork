#include "SchroederAllpass.h"
#include "ReverbNetworkDefines.h"
#include <cmath>
#include <string>

SchroederAllpass::SchroederAllpass(double sampleRate, double delay, double decay)
	: inputBuffer(nullptr)
	, outputBuffer(nullptr)
	, bufferPos(0) 
	, sampleRate(sampleRate)
	, delaySamples(sampleRate * delay)
	, delayTime(delay)
	, decayTime(decay)
{
	yn = 0;
	xnD = 0;
	ynD = 0;
	setDecayTime(decay);
	createBuffers();
}

SchroederAllpass::~SchroederAllpass() {
	freeBuffers();
}

void SchroederAllpass::doProcessing(double& sample) {
	// Read current sample
	//xn = sample; // x(n)
	// Read stored samples from circular buffers
	xnD = inputBuffer[bufferPos]; // x(n-D)
	ynD = outputBuffer[bufferPos]; // y(n-D)

	// Difference equation
	yn = -gain*sample + xnD + gain*ynD;

	// Store current input sample in circular buffer
	inputBuffer[bufferPos] = sample;

	// Write output sample
	sample = yn;

	// Store current output sample in circular buffer
	outputBuffer[bufferPos] = yn;

	// Increment index of circular buffers
	++bufferPos;

	// Reset index if it has exceeded the delay in samples
	if (bufferPos >= delaySamples) {
		bufferPos = 0;
	}
	//FILE* pFile = fopen("E:\\logVst.txt", "a");
	////fprintf(pFile, "y(n): %s\n", std::to_string(delayTime).c_str());
	////fprintf(pFile, "y(n): %s\n", std::to_string(decayTime).c_str());
	//fprintf(pFile, "y(n): %s\n", std::to_string(gain).c_str());
	//fclose(pFile);
}

void SchroederAllpass::createBuffers() {
	freeBuffers(); // just in case...

	// Create pre-initialized arrays for maximum delay value (round up)
	inputBuffer = new double[(int)(std::ceil(sampleRate * (MAXDELAY / 1000.0)))]();
	outputBuffer = new double[(int)(std::ceil(sampleRate * (MAXDELAY / 1000.0)))]();
}

void SchroederAllpass::freeBuffers() {
	bufferPos = 0;

	if (inputBuffer) {
		delete[] inputBuffer;
		inputBuffer = nullptr; // !!! Otherwise crash when checking if (inputBuffer)
	}
	if (outputBuffer) {
		delete[] outputBuffer;
		outputBuffer = nullptr; // !!!
	}
}

void SchroederAllpass::setDelayTimeMsec(const double& ms) {
	delayTime = ms / 1000; 
	delaySamples = (unsigned long)(delayTime * sampleRate); 
	calculateGain();
}

void SchroederAllpass::setDecayTime(const double& sec) {
	decayTime = sec;
	calculateGain();
}

void SchroederAllpass::calculateGain() {
	double dB = 0.0;
	// Prevent division by zero
	if (decayTime > 0.0) {
		dB = -60 * (delayTime / decayTime);
	}
	gain = pow(10.0, dB / 20);
}