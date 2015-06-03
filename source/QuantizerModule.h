#ifndef QUANTIZERMODULE_H
#define QUANTIZERMODULE_H

class QuantizerModule
{
public:
	QuantizerModule(unsigned int quantization);
	~QuantizerModule();

	void processSample(double& sample);
	void setQuantization(const double& q);

private:
	unsigned int bitsToReset;
	// (min.)32 bit mask
	long int mask;

};

#endif // QUANTIZERMODULE_H