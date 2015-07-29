#include "SignalGenerator.h"
#include "ValueConversion.h"
#include "ReverbNetworkDefines.h"

SignalGenerator::SignalGenerator(const SignalGeneratorType& signalType) 
	: signalType(signalType) {

	gainFactor = ValueConversion::logToLinear(DEF_SIGNALGENERATOR_AMPLITUDE);
	width = DEF_SIGNALGENERATOR_WIDTH;
	autoFireEnabled = true;
	autoTimeInSamples = DEF_SIGNALGENERATOR_TIME * ValueConversion::getSampleRate();
	fire = DEF_SIGNALGENERATOR_FIRE;

	autoFireCounter = 0;
	fireCounter = 0;
	sample = 0.0;
}

void SignalGenerator::setGain(const double& gainInDB) {
	this->gainFactor = ValueConversion::logToLinear(gainInDB);
	/*FILE* pFile = fopen("E:\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", std::to_string(gainFactor).c_str());
	fclose(pFile);*/
}

void SignalGenerator::setAutoTime(const double& autoTimeInSec) {
	this->autoTimeInSamples = std::round(autoTimeInSec * ValueConversion::getSampleRate());
}

double& SignalGenerator::generateSample() {
	sample = 0.0;
	if (autoFireEnabled) {
		// Output samples until the width has been reached
		if (autoFireCounter < width) {
			sample = gainFactor * 1.0;
		}
		++autoFireCounter;
		// Reset the counter when the auto time has been reached
		if (autoFireCounter >= autoTimeInSamples) {
			autoFireCounter = 0;
		}
		/*FILE* pFile = fopen("E:\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", std::to_string(sample).c_str());
		fclose(pFile);*/
	}
	if (fire) {
		fireCounter = width;
		fire = false;
	}
	if (fireCounter > 0) {
		sample = gainFactor * 1.0;
		--fireCounter;
	}
	return sample;
}