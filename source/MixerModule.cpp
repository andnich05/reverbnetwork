#include "MixerModule.h"
#include "ReverbNetworkDefines.h"
#include <string>

MixerModule::MixerModule(const double& defaultGain) {
	// Initialization
	inputGain.resize(MAXINPUTS, defaultGain);
	inputMuted.resize(MAXINPUTS, false);
	inputSoloed.resize(MAXINPUTS, false);
	isAnyOtherInputSoloed = false;
}

MixerModule::~MixerModule() {
}

double MixerModule::mixInputs(double* moduleInputBuffer, double* vstInputBuffer) {
	if (moduleInputBuffer == nullptr || vstInputBuffer == nullptr) {
		return 0.0;
	}

	// Most time consuming operations
	double output = 0.0;
	for (int i = 0; i < MAXINPUTS; ++i) {
		// If input gain is zero then there is no sense in doing anything
		if (inputGain[i] != 0.0) {
			if (inputMuted[i] != false) { // Mute has precedence
				if (inputSoloed[i] == true) {
					if (i < MAXMODULENUMBER) {
						output += moduleInputBuffer[i] * inputGain[i];
					}
					else {
						output += vstInputBuffer[i - MAXMODULENUMBER] * inputGain[i];
					}
				}
				else {
					if (isAnyOtherInputSoloed == false) {
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
	}

	return output;
}

void MixerModule::setInputSoloed(const int& input, const bool& solo) {
	inputSoloed[input] = solo;
	if (solo) {
		isAnyOtherInputSoloed = true;
	}
	else {
		isAnyOtherInputSoloed = false;
		for (int i = 0; i < MAXINPUTS; ++i) {
			if (inputSoloed[i]) {
				isAnyOtherInputSoloed = true;
				break;
			}
		}
	}
}

//bool MixerModule::isAnyOtherInputSoloed(const int& currentInput) {
//	bool result = false;
//	for (int i = 0; i < MAXINPUTS; ++i) {
//		/*FILE* pFile = fopen("E:\\logVst.txt", "a");
//		fprintf(pFile, "y(n): %s\n", std::to_string(inputSoloed[i]).c_str());
//		fclose(pFile);*/
//		if (i == currentInput) continue;
//		if (inputSoloed[i] == true) {
//			result = true;
//			break;
//		}
//	}
//	
//	return result;
//}