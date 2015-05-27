#ifndef QUANTIZERMODULE_H
#define QUANTIZERMODULE_H

class QuantizerModule
{
public:
	QuantizerModule(unsigned int quantization);
	~QuantizerModule();

	void processSample(double& sample);

private:
	unsigned int quantization;

};

#endif // QUANTIZERMODULE_H