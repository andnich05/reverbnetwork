#include "MixerModule.h"
#include "ReverbNetworkDefines.h"
#include <string>

MixerModule::MixerModule(const double& defaultGain) {
	// Initialization
	inputGain.resize(MAXMODULENUMBER + MAXVSTINPUTS, defaultGain);
}

MixerModule::~MixerModule() {
}

double MixerModule::mixInputs(double* moduleInputBuffer, std::vector<double>& vstInputBuffer) {
	if (moduleInputBuffer == nullptr) {
		return 0.0;
	}

	double output = 0.0;
	for (int i = 0; i < inputGain.size(); ++i) {
		if (inputGain[i] != 0.0) {
			if (i < MAXMODULENUMBER) {
				output += moduleInputBuffer[i] * inputGain[i];
			}
			else {
				output += vstInputBuffer[i - MAXMODULENUMBER] * inputGain[i];
			}
		}
	}

	return output;
}

void MixerModule::setInputGain(const int& input, const double& gain) {
	inputGain[input] = gain;
}