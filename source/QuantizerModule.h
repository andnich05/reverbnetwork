#ifndef QUANTIZERMODULE_H
#define QUANTIZERMODULE_H

class QuantizerModule
{
public:
	QuantizerModule(unsigned int quantization);
	~QuantizerModule();

	// Process sample by reference
	void processSample(double& sample) const;
	// Set the quantization from 1 bit to 32 bit
	void setQuantization(const double& q);

private:
	// Calculate the appropriate factor depending on the quantization method
	void calculateFactor();

	unsigned int bitsToReset; // 32 - quantization
	long int mask; // (min.)32 bit mask
	double factor;

};

#endif // QUANTIZERMODULE_H