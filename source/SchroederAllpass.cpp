#include "SchroederAllpass.h"
#include "ReverbNetworkDefines.h"
#include <cmath>

SchroederAllpass::SchroederAllpass(unsigned long sampleRate, unsigned long delaySamples, double gain)
	: inputBuffer(nullptr)
	, outputBuffer(nullptr)
	, bufferPos(0) 
	, sampleRate(sampleRate)
	, delaySamples(delaySamples)
	, delayTime(0.0)
	, decayTime(0.0)
	, gain(gain)
{
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

	// Write output sample (use reference(?))
	sample = yn;

	// Store current output sample in circular buffer
	outputBuffer[bufferPos] = yn;

	// Increment index of circular buffers
	++bufferPos;

	// Reset index if it has exceeded the delay in samples
	if (bufferPos >= delaySamples) {
		bufferPos = 0;
	}
}

void SchroederAllpass::createBuffers() {
	freeBuffers(); // just in case...

	// Create pre-initialized arrays for maximum delay value (round up)
	inputBuffer = new double[(int)(std::ceil(sampleRate * MAXDELAY))]();
	outputBuffer = new double[(int)(std::ceil(sampleRate * MAXDELAY))]();
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

//#include <string>
void SchroederAllpass::setDecayTime(const double& sec) {
	decayTime = sec;
	double db = -60 * (delayTime / decayTime);
	gain = pow(10.0, db / 20);

	/*FILE* pFile = fopen("E:\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", std::to_string(delayTime).c_str());
	fprintf(pFile, "y(n): %s\n", std::to_string(decayTime).c_str());
	fprintf(pFile, "y(n): %s\n", std::to_string(gain).c_str());
	fclose(pFile);*/
}