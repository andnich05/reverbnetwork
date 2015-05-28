#include "QuantizerModule.h"
#include <bitset>
#include <cmath>

const double max32bitValueSigned = pow(2, 32) / 2 - 1;
const double min32bitValueSigned = pow(2, 32) / 2;

// ToDo: AND Maske
// ToDo: Bios Gain 

QuantizerModule::QuantizerModule(unsigned int quantization) {
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

	/*FILE* pFile = fopen("E:\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", std::to_string(sample).c_str());
	fclose(pFile);*/

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

	// Create bitset vector
	std::bitset<32> s (temp);

	/*FILE* pFile = fopen("E:\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", std::to_string(temp).c_str());
	fprintf(pFile, "y(n): %s\n", (s.to_string()).c_str());
	fclose(pFile);*/

	// Set bits to zero
	for (unsigned int i = 0; i < bitsToReset; ++i) {
		s.reset(i);
	}

	// Convert back to an 32 bit integer
	temp = (long int)(s.to_ulong());

	// Convert the integer back to a double
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