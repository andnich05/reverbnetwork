#include "EqualizerModule.h"
#define _USE_MATH_DEFINES
#include <cmath>

EqualizerModule::EqualizerModule(unsigned int samplingFreq, unsigned int centerFreq, double qFactor, double gain)
	: samplingFreq(samplingFreq)
	, centerFreq(centerFreq)
	, qFactor(qFactor)
	, gain(gain)
{
	
}

EqualizerModule::~EqualizerModule() 
{
}

void EqualizerModule::calculateK() {
	double cutoffFreq = centerFreq / (2 * qFactor);
	K = tan((cutoffFreq / samplingFreq) * M_PI);
}

void EqualizerModule::calculateCoefficients() {
	// Tiefpass
	a0 = (pow(K, 2)) / (1 + sqrt(2) * K + pow(K, 2));
	a1 = (2 * pow(K, 2)) / (1 + sqrt(2) * K + pow(K, 2));
	a2 = a0;
	b1 = (2 * (pow(K, 2) - 1)) / (1 + sqrt(2) * K + pow(K, 2));
	b2 = (1 - sqrt(2) * K + pow(K, 2)) / (1 + sqrt(2) * K + pow(K, 2));
}

void EqualizerModule::process(double& sample) {

}