#include "GainModule.h"
#include "ReverbNetworkDefines.h"

GainModule::GainModule() 
	: gain(0) {

}

GainModule::~GainModule() {

}

void GainModule::processSample(double& sample) {
	sample *= gain;

#ifdef LIMITER
	if (sample > 1.0) {
		sample = 1.0;
	}
#endif
}