#include "GainModule.h"
#include "ReverbNetworkDefines.h"
#include <string>
GainModule::GainModule(double gain) 
	: gain(gain) {
	FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", std::to_string(gain).c_str());
	fclose(pFile);
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