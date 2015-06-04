#include "QuantizerModule.h"
#include <bitset>
#include <cmath>

const double max32bitValueSigned = pow(2, 32) / 2 - 1;
const double min32bitValueSigned = pow(2, 32) / 2;

QuantizerModule::QuantizerModule(unsigned int quantization) 
	: mask(-1) {
	setQuantization(quantization);
}

QuantizerModule::~QuantizerModule() {

}

void QuantizerModule::setQuantization(const double& q) {
	bitsToReset = 32 - (unsigned int)(std::round(q));
	/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", std::to_string(q).c_str());
	fprintf(pFile, "y(n): %s\n", std::to_string(bitsToReset).c_str());
	fclose(pFile);*/
}

void QuantizerModule::processSample(double& sample) {

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

// Method 1: with bit shifting
//--------------------------------
	// Convert signed to unsigned
	/*long unsigned tu = temp + min32bitValueSigned;

	// Reset bits
	mask = -1; // ...111111
	mask <<= bitsToReset; // ...111000
	tu &= mask;

	// Calculate "spread factor"
	double factor = (pow(2.0, (double)bitsToReset) - 1.0) / (pow(2.0, 32.0 - (double)bitsToReset) - 1.0);

	// Shift bits which has not been resetted to the right
	long int temp2 = tu >> bitsToReset;
	if (tu < 0) {
		// Make sure zeros are added when shifting right
		long int shift = -1;
		shift <<= (32 - bitsToReset);
		temp2 ^= shift;
	}

	// Mulitply spread factor with shifted samples
	long int toOr = (long int)(std::round(factor * (double)(temp2)));
	// unsigned masked sample OR spreaded number
	tu = tu | toOr;
	
	// Convert back to signed
	temp = tu - min32bitValueSigned;*/
//------------------------------------

// Method 2: Without bit shifting
//------------------------------------
	// Convert signed to unsigned
	long unsigned tu = temp + min32bitValueSigned;

	// Those variables are fixed for the specified bitsToReset! => needs to be done only one time!
	unsigned long newMax = (pow(2, 32 - bitsToReset) - 1) * (pow(2, bitsToReset) - 1);
	double factor = (pow(2, 32) - 1) / newMax;
	//double lg2f = log2f(factor);
	unsigned long n = factor * (pow(2, bitsToReset) - 1);
	//---

	// Divide the sample, round it and multiply it again
	tu = (long unsigned)(std::round((double)tu / n)) * n;
	
	// Convert back to signed
	temp = tu - min32bitValueSigned;
//-----------------------------------

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