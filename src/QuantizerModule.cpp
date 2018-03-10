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

#include "../include/QuantizerModule.h"
#include <bitset>
#include <cmath>

// Maximum values of a 32 bit signed integer
const double max32bitValueSigned = pow(2, 32) / 2 - 1;
const double min32bitValueSigned = pow(2, 32) / 2;

QuantizerModule::QuantizerModule(unsigned int quantization) 
	: bitsToReset(0)
	, mask(-1)
	, factor(0.0)
	, bitCorretionMethod(BitCorrectionMethod::withoutScaling)
{
	setQuantization(quantization);
}

QuantizerModule::~QuantizerModule() {

}

void QuantizerModule::setQuantization(double q) {
	bitsToReset = 32 - (unsigned int)(std::round(q));
	calculateFactor();
}

void QuantizerModule::processSample(double& sample) const {
	// Convert to 32 bit signed integer
	long int temp = 0;
	if (sample > 1.0) {
		temp = (long int)max32bitValueSigned;
	}
	else if (sample < -1.0) {
		temp = -(long int)max32bitValueSigned;
	}
	else {
		temp = (long int)(sample * max32bitValueSigned);
	}

	if (bitCorretionMethod == BitCorrectionMethod::withBitShifting) {
		// Method 1: with bit shifting
		//--------------------------------
		// Convert signed to unsigned
		unsigned long tu = temp + (unsigned long)max32bitValueSigned;

		tu &= mask;

		// Shift bits which has not been resetted to the right
		unsigned long int shifted = tu >> bitsToReset;

		// Mulitply spread factor with shifted samples
		unsigned long int toOr = (long int)(std::round(factor * (double)(shifted)));
		// unsigned masked sample <OR> spreaded number
		tu = tu | toOr;

		// Convert back to signed
		temp = tu - (unsigned long)min32bitValueSigned;
		//------------------------------------
	}
	else if (bitCorretionMethod == BitCorrectionMethod::withoutBitShifting) {
		// Method 2: Without bit shifting
		//------------------------------------
		if (bitsToReset < 32) {
			// Convert signed to unsigned
			unsigned long tu = temp + (unsigned long)max32bitValueSigned;

			// Divide the sample, round it and multiply it again
			tu = (unsigned long)((unsigned long)(std::round((double)tu / factor)) * factor);

			// Convert back to signed
			temp = tu - (unsigned long)min32bitValueSigned;
		}
		//-----------------------------------
	}
	else if (bitCorretionMethod == BitCorrectionMethod::withoutScaling) {
		// Method 3: with bit shifting, without scaling
		//--------------------------------
		// Clear the bits of the sample
		temp &= mask;
		// Shift the sample relative to 0.0 (remove DC offset)
		temp += (long int)factor;
		//------------------------------------
	}
	else
	{
		// ?
	}

	// Convert the integer back to double
	if (temp > max32bitValueSigned) {
		sample = 1.0;
	}
	else if (temp < -max32bitValueSigned) {
		sample = -1.0;
	}
	else {
		sample = (double)temp / (double)max32bitValueSigned;
	}
}

void QuantizerModule::calculateFactor() {
	switch (bitCorretionMethod) {
	case BitCorrectionMethod::withBitShifting: 
		// Reset bits
		mask = -1; // ...111111
		mask <<= bitsToReset; // ...111000
		// Calculate "spread factor"
		factor = (pow(2.0, (double)bitsToReset) - 1.0) / (pow(2.0, 32.0 - (double)bitsToReset) - 1.0);
		break;
	case BitCorrectionMethod::withoutBitShifting:
		factor = (pow(2.0, 32.0) - 1.0) / (pow(2.0, 32.0 - (double)bitsToReset) - 1.0);
		break;
	case BitCorrectionMethod::withoutScaling:
	{
		// Set the mask back to default
		mask = -1; // ...111111
		// Shift the mask
		mask <<= bitsToReset; // ...111000
		// Get the maximum value which can occur
		unsigned long maxValue = (unsigned long)mask;
		// Calculate the value wich has to be added to every sample in order to correct the asymmetry
		factor = (pow(2, 32) - maxValue) / 2;
		break;
	}
	default:
		break;
	}
}
