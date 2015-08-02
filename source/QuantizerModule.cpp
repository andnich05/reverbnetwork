/*
* QuantizerModule: Requantize the input signal to a new bitdepth
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

#include "QuantizerModule.h"
#include <bitset>
#include <cmath>

// Maximum values of a 32 bit signed integer
const double max32bitValueSigned = pow(2, 32) / 2 - 1;
const double min32bitValueSigned = pow(2, 32) / 2;

// Quantization methods in order to correct the signal asymmetry, each has it's benefits and drawbacks
enum BitCorrectionMethod {
	withBitShifting, // Fast, many operations, complex, scales the signal to full range
	withoutBitShifting, // Relatively fast, only a few operations, simple, scales the signal to full range
	withoutScaling // Very fast, few operations, simple, doesn't scale the signal to full range
};

#define BITCORRECTIONMETHOD withoutScaling

QuantizerModule::QuantizerModule(unsigned int quantization) 
	: mask(-1), factor(0.0) {
	setQuantization(quantization);
}

QuantizerModule::~QuantizerModule() {

}

void QuantizerModule::setQuantization(const double& q) {
	bitsToReset = 32 - (unsigned int)(std::round(q));
	calculateFactor();
}

void QuantizerModule::processSample(double& sample) const {
	// Convert to 32 bit signed integer
	long int temp = 0;
	if (sample >= 0.0) {
		// Check for out of range sample values
		if (sample <= 1.0) {
			temp = (long int)(sample * max32bitValueSigned);
		}
		else {
			temp = (long int)max32bitValueSigned;
		}
	}
	else {
		// Check for out of range sample values
		if (sample >= -1.0) {
			temp = (long int)(sample * min32bitValueSigned);
		}
		else {
			temp = (long int)min32bitValueSigned;
		}
	}

	if (BITCORRECTIONMETHOD == withBitShifting) {
		// Method 1: with bit shifting
		//--------------------------------
		// Convert signed to unsigned
		unsigned long tu = temp + (unsigned long)min32bitValueSigned;

		tu &= mask;

		// Shift bits which has not been resetted to the right
		unsigned long int shifted = tu >> bitsToReset;

		// Mulitply spread factor with shifted samples
		unsigned long int toOr = (long int)(std::round(factor * (double)(shifted)));
		// unsigned masked sample OR spreaded number
		tu = tu | toOr;

		// Convert back to signed
		temp = tu - (unsigned long)min32bitValueSigned;
		//------------------------------------
	}
	else if (BITCORRECTIONMETHOD == withoutBitShifting) {
		// Method 2: Without bit shifting
		//------------------------------------
		if (bitsToReset < 32) {
			// Convert signed to unsigned
			unsigned long tu = temp + (unsigned long)min32bitValueSigned;

			// Divide the sample by the new number n, round it and multiply it with n again
			tu = (unsigned long)((unsigned long)(std::round((double)tu / factor)) * factor);

			// Convert back to signed
			temp = tu - (unsigned long)min32bitValueSigned;
		}
		//-----------------------------------
	}
	else if (BITCORRECTIONMETHOD == withoutScaling) {
		// Method 3: with bit shifting, without scaling (only shifting)
		//--------------------------------
		// Clear the bits of the sample
		temp &= mask;
		// Shift the sample relative to 0.0 (remove DC offset)
		temp += (long int)factor;
		//------------------------------------
	}

	// Convert the integer back to double
	if (temp >= 0) {
		if (temp <= (long int)(max32bitValueSigned)) {
			sample = (double)temp / (double)max32bitValueSigned;
		}
		else {
			sample = 1.0;
		}
	}
	else {
		if (temp >= (long int)(-min32bitValueSigned)) {
			sample = (double)temp / (double)min32bitValueSigned;
		}
		else {
			sample = -1.0;
		}
	}

		/*pFile = fopen("E:\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", std::to_string(sample).c_str());
		fclose(pFile);*/
}

void QuantizerModule::calculateFactor() {
	switch (BITCORRECTIONMETHOD) {
	case withBitShifting: 
		// Reset bits
		mask = -1; // ...111111
		mask <<= bitsToReset; // ...111000
		// Calculate "spread factor"
		factor = (pow(2.0, (double)bitsToReset) - 1.0) / (pow(2.0, 32.0 - (double)bitsToReset) - 1.0);
		break;
	case withoutBitShifting: 
		factor = (pow(2.0, 32.0) - 1.0) / (pow(2.0, 32.0 - (double)bitsToReset) - 1);
		break;
	case withoutScaling:
		// Set the mask back to default
		mask = -1; // ...111111
		// Shift the mask
		mask <<= bitsToReset; // ...111000
		// Get the maximum value which can occur
		unsigned long maxValue = 0 | mask;
		// Calculate the value wich has to be added to every sample in order to correct the asymmetry
		factor = ((pow(2, 32) - 1) - maxValue) / 2;
		break;
	}
}