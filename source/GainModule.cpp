#include "GainModule.h"
#include "ReverbNetworkDefines.h"
#include <string>
GainModule::GainModule(double gain) 
	: gain(gain) {

}

GainModule::~GainModule() {

}

void GainModule::processSample(double& sample) {
	sample *= gain;

#ifdef LIMITER
	if (sample > 1.0) {
		sample = 1.0;
	}
	else if (sample < -1.0) {
		sample = -1.0;
	}
#endif
}