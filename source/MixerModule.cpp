/*
* MixerModule: Mix several input signals into one
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

#include "MixerModule.h"
#include "ReverbNetworkDefines.h"
#include <string>

MixerModule::MixerModule(const double& defaultGain) {
	// Initialization
	inputGain.resize(MAXINPUTS, defaultGain);
	inputMuted.resize(MAXINPUTS, false);
	inputSoloed.resize(MAXINPUTS, false);
	isAnyInputSoloed = false;
}

MixerModule::~MixerModule() {
}

double MixerModule::mixInputs(double* moduleInputBuffer, double* vstInputBuffer, double& signalGeneratorSample) const {
	// Return if the pointers are empy
	if (moduleInputBuffer == nullptr || vstInputBuffer == nullptr) {
		return 0.0;
	}

	// Most time consuming operations of the whole plugin!
	double output = 0.0;
	for (int i = 0; i < MAXINPUTS; ++i) {
		// If input gain is zero then there is no sense in doing anything (saves a LOT of processing time)
		if (inputGain[i] != 0.0) {
			if (inputMuted[i] == false) { // Mute has precedence
				if (inputSoloed[i] == true) { // If soloed: process regardless of the other inputs
					// Check if the current input is a module, VST input or the signal generator
					if (i < MAXMODULENUMBER) {
						output += moduleInputBuffer[i] * inputGain[i];
					}
					else if (i - MAXMODULENUMBER < MAXVSTINPUTS) {
						output += vstInputBuffer[i - MAXMODULENUMBER] * inputGain[i];
					}
					else {
						output += signalGeneratorSample * inputGain[i];
					}
				}
				else {
					// If the input is not soloed process the sample only if no other input is soloed
					if (isAnyInputSoloed == false) {
						if (i < MAXMODULENUMBER) {
							output += moduleInputBuffer[i] * inputGain[i];
						}
						else if (i - MAXMODULENUMBER < MAXVSTINPUTS) {
							output += vstInputBuffer[i - MAXMODULENUMBER] * inputGain[i];
						}
						else {
							output += signalGeneratorSample * inputGain[i];
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
	// Check if any other input is soloed when the current input is being been unsoloed
	if (solo) {
		isAnyInputSoloed = true;
	}
	else {
		isAnyInputSoloed = false;
		for (int i = 0; i < MAXINPUTS; ++i) {
			if (inputSoloed[i]) {
				isAnyInputSoloed = true;
				break;
			}
		}
	}
}