#include "GainModule.h"
#include "ReverbNetworkDefines.h"

GainModule::GainModule(double gain) 
	: gain(gain) {

}

GainModule::~GainModule() {

}

void GainModule::processSample(double& sample) const {
	sample *= gain;

#ifdef OUTPUTLIMITER
	if (sample > 1.0) {
		sample = 1.0;
	}
	else if (sample < -1.0) {
		sample = -1.0;
	}
#endif
}