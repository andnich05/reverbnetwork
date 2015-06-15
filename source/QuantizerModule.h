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
	void calculateFactor();

	unsigned int bitsToReset;
	// (min.)32 bit mask
	long int mask;
	double factor;

};

#endif // QUANTIZERMODULE_H