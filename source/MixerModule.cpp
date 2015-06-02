#include "MixerModule.h"
#include "ReverbNetworkDefines.h"
#include <string>

MixerModule::MixerModule(const double& defaultGain) {
	// Initialization
	inputGain.resize(MAXINPUTS, defaultGain);
	inputMuted.resize(MAXINPUTS, false);
	inputSoloed.resize(MAXINPUTS, false);
}

MixerModule::~MixerModule() {
}

double MixerModule::mixInputs(double* moduleInputBuffer, std::vector<double>& vstInputBuffer) {
	if (moduleInputBuffer == nullptr) {
		return 0.0;
	}


	double output = 0.0;
	for (int i = 0; i < MAXINPUTS; ++i) {
		/*FILE* pFile = fopen("E:\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", std::to_string(inputMuted[i]).c_str());
		fclose(pFile);*/
		if (inputMuted[i] == false) { // Mute has precedence
			if (inputSoloed[i] == true || isAnyOtherInputSoloed(i) == false) {
				if (inputGain[i] != 0.0) {
					if (i < MAXMODULENUMBER) {
						output += moduleInputBuffer[i] * inputGain[i];
					}
					else {
						output += vstInputBuffer[i - MAXMODULENUMBER] * inputGain[i];
					}
				}
			}
		}
	}

	return output;
}

bool MixerModule::isAnyOtherInputSoloed(const int& currentInput) {
	bool result = false;
	for (int i = 0; i < MAXINPUTS; ++i) {
		/*FILE* pFile = fopen("E:\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", std::to_string(inputSoloed[i]).c_str());
		fclose(pFile);*/
		if (i == currentInput) continue;
		if (inputSoloed[i] == true) {
			result = true;
			break;
		}
	}
	
	return result;
}