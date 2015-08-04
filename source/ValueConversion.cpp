/*
* ValueConversion: Conversion functions from/to plain/norm values
*
* Copyright (C) 2015  Andrej Nichelmann
*                     Klaus Michael Indlekofer
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ValueConversion.h"
#include "ReverbNetworkDefines.h"
#include <cstdlib>
#include <string>
#include "ReverbNetworkEnums.h"

double ValueConversion::sampleRate = 44100.0;

ValueConversion::ValueConversion() {
	
}

ValueConversion::~ValueConversion() {
}

double ValueConversion::normToPlainMixerInputSelect(const double& normValue) {
	// Round!
	return std::round(normValue * ((double)MAXINPUTS)); // incl. index 0
}

double ValueConversion::plainToNormMixerInputSelect(const double& plainValue) {
	return plainValue * (1 / ((double)MAXINPUTS)); // incl. index 0
}

double ValueConversion::normToPlainInputGain(const double& normValue) {
	return (MAX_MIXERGAIN - MIN_MIXERGAIN) * normValue + MIN_MIXERGAIN;
}

double ValueConversion::plainToNormInputGain(const double& plainValue) {
	return (plainValue - MIN_MIXERGAIN) / (MAX_MIXERGAIN - MIN_MIXERGAIN);
}

double ValueConversion::normToPlainFilterTypeSelect(const double& normValue) {
	return std::round(normValue * ((double)(FilterType::numberOfFilterTypes - 1)));
}

double ValueConversion::plainToNormFilterTypeSelect(const double& plainValue) {
	return plainValue * (1 / ((double)(FilterType::numberOfFilterTypes - 1)));
}

double ValueConversion::normToPlainQuantization(const double& normValue) {
	return std::round(((double)MAX_QUANTIZERBITDEPTH - (double)MIN_QUANTIZERBITDEPTH) * normValue + (double)MIN_QUANTIZERBITDEPTH);
}

double ValueConversion::plainToNormQuantization(const double& plainValue) {
	return ((double)plainValue - (double)MIN_QUANTIZERBITDEPTH) / ((double)MAX_QUANTIZERBITDEPTH - (double)MIN_QUANTIZERBITDEPTH);
}

double ValueConversion::normToPlainProcCenterFreq(const double& normValue) {
	return (getMaxEqFrequency() - MIN_EQCENTERFREQ) * normValue + MIN_EQCENTERFREQ;
}

double ValueConversion::plainToNormProcCenterFreq(const double& plainValue) {
	return (plainValue - MIN_EQCENTERFREQ) / (getMaxEqFrequency() - MIN_EQCENTERFREQ);
}

double ValueConversion::normToPlainQFactor(const double& normValue) {
	return (MAX_EQQFACTOR - MIN_EQQFACTOR) * normValue + MIN_EQQFACTOR;
}

double ValueConversion::plainToNormQFactor(const double& plainValue) {
	return (plainValue - MIN_EQQFACTOR) / (MAX_EQQFACTOR - MIN_EQQFACTOR);
}

double ValueConversion::normToPlainEqGain(const double& normValue) {
	return (MAX_EQGAIN - MIN_EQGAIN) * normValue + MIN_EQGAIN;
}

double ValueConversion::plainToNormEqGain(const double& plainValue) {
	return (plainValue - MIN_EQGAIN) / (MAX_EQGAIN - MIN_EQGAIN);
}

double ValueConversion::normToPlainEqCoefficients(const double& normValue) {
	return (MAX_EQCOEFFICIENTS - MIN_EQCOEFFICIENTS) * normValue + MIN_EQCOEFFICIENTS;
}

double ValueConversion::plainToNormEqCoefficients(const double& plainValue) {
	return (plainValue - MIN_EQCOEFFICIENTS) / (MAX_EQCOEFFICIENTS - MIN_EQCOEFFICIENTS);
}

double ValueConversion::normToPlainDelay(const double& normValue) {
	return (MAX_ALLPASSDELAY - MIN_ALLPASSDELAY) * normValue + MIN_ALLPASSDELAY;
}

double ValueConversion::plainToNormDelay(const double& plainValue) {
	return (plainValue - MIN_ALLPASSDELAY) / (MAX_ALLPASSDELAY - MIN_ALLPASSDELAY);
}

double ValueConversion::delayMillisecondsToSamples(const double& delayMilliseconds) {
	return delayMilliseconds * sampleRate / 1000;
}

double ValueConversion::delaySamplesToMilliseconds(const double& delaySamples) {
	if (sampleRate == 0.0) {
		return 0.0;
	}
	return delaySamples / sampleRate * 1000;
}

double ValueConversion::calculateDiffK(const double& delayInMs, const double& decayInMs) {
	if (decayInMs == 0.0) return 0.0;
	if (delayInMs == 0.0) return 1.0;
	if (decayInMs > 0.0) {
		return pow(10, (-60 * (delayInMs / decayInMs) / 20));
	}
	else {
		return -pow(10, (-60 * (delayInMs / -decayInMs) / 20));
	}
}

double ValueConversion::diffKToDecay(const double& diffK, const double& delayInMs) {
	double absDiffK = std::abs(diffK);
	if (absDiffK == 1.0) return MAX_ALLPASSDECAY;
	if (absDiffK == 0.0) return 0.0;
	if (absDiffK > 1.0) return 0.0;
	if (diffK > 0.0) {
		return (-60 / (20 * log10(diffK))) * delayInMs;
	}
	else {
		return -(-60 / (20 * log10(-diffK))) * delayInMs;
	}
}

double ValueConversion::normToPlainDecay(const double& normValue) {
	return (MAX_ALLPASSDECAY - MIN_ALLPASSDECAY) * normValue + MIN_ALLPASSDECAY;
}

double ValueConversion::plainToNormDecay(const double& plainValue) {
	return (plainValue - MIN_ALLPASSDECAY) / (MAX_ALLPASSDECAY - MIN_ALLPASSDECAY);
}

double ValueConversion::normToPlainModExcursion(const double& normValue) {
	return (MAX_ALLPASSMODEXCURSION - MIN_ALLPASSMODEXCURSION) * normValue + MIN_ALLPASSMODEXCURSION;
}

double ValueConversion::plainToNormModExcursion(const double& plainValue) {
	return (plainValue - MIN_ALLPASSMODEXCURSION) / (MAX_ALLPASSMODEXCURSION - MIN_ALLPASSMODEXCURSION);
}

double ValueConversion::normToPlainModRate(const double& normValue) {
	return (MAX_ALLPASSMODRATE - MIN_ALLPASSMODRATE) * normValue + MIN_ALLPASSMODRATE;
}

double ValueConversion::plainToNormModRate(const double& plainValue) {
	return (plainValue - MIN_ALLPASSMODRATE) / (MAX_ALLPASSMODRATE - MIN_ALLPASSMODRATE);
}

double ValueConversion::normToPlainOutputGain(const double& normValue) {
	return (MAX_OUTPUTGAIN - MIN_OUTPUTGAIN) * normValue + MIN_OUTPUTGAIN;
}

double ValueConversion::plainToNormOutputGain(const double& plainValue) {
	return (plainValue - MIN_OUTPUTGAIN) / (MAX_OUTPUTGAIN - MIN_OUTPUTGAIN);
}

double ValueConversion::normToPlainSignalType(const double& normValue) {
	//return std::round((MAX_SIGNALGENERATOR_SIGNALTYPE - MIN_SIGNALGENERATOR_SIGNALTYPE) * normValue + MIN_SIGNALGENERATOR_SIGNALTYPE);
	return 0.0;
}

double ValueConversion::plainToNormSignalType(const double& plainValue) {
	//return (plainValue - MIN_SIGNALGENERATOR_SIGNALTYPE) / (MAX_SIGNALGENERATOR_SIGNALTYPE - MIN_SIGNALGENERATOR_SIGNALTYPE);
	// Only one entry in the menu so always return 0.0
	return 0.0;
}

double ValueConversion::normToPlainSignalAmplitude(const double& normValue) {
	return (MAX_SIGNALGENERATOR_AMPLITUDE - MIN_SIGNALGENERATOR_AMPLITUDE) * normValue + MIN_SIGNALGENERATOR_AMPLITUDE;
}

double ValueConversion::plainToNormSignalAmplitude(const double& plainValue) {
	return (plainValue - MIN_SIGNALGENERATOR_AMPLITUDE) / (MAX_SIGNALGENERATOR_AMPLITUDE - MIN_SIGNALGENERATOR_AMPLITUDE);
}

double ValueConversion::normToPlainSignalWidth(const double& normValue) {
	return std::round((MAX_SIGNALGENERATOR_WIDTH - MIN_SIGNALGENERATOR_WIDTH) * normValue + MIN_SIGNALGENERATOR_WIDTH);
}

double ValueConversion::plainToNormSignalWidth(const double& plainValue) {
	return (plainValue - MIN_SIGNALGENERATOR_WIDTH) / (MAX_SIGNALGENERATOR_WIDTH - MIN_SIGNALGENERATOR_WIDTH);
}

double ValueConversion::normToPlainSignalTime(const double& normValue) {
	return (MAX_SIGNALGENERATOR_TIME - MIN_SIGNALGENERATOR_TIME) * normValue + MIN_SIGNALGENERATOR_TIME;
}

double ValueConversion::plainToNormSignalTime(const double& plainValue) {
	return (plainValue - MIN_SIGNALGENERATOR_TIME) / (MAX_SIGNALGENERATOR_TIME - MIN_SIGNALGENERATOR_TIME);
}

double ValueConversion::linearToLog(const double& linearValue) {
	if (linearValue > 0.0) {
		return 20 * log10(linearValue);
	}
	else {
		return 0.0;
	}
}

double ValueConversion::logToLinear(const double& logValue) {
	return pow(10, (logValue / 20));
}

bool ValueConversion::textEditStringToValueConversion(const char* txt, float& result, void* userData) {
	/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", std::to_string(888).c_str());
	fclose(pFile);*/
	result = (float)(atof(txt));
	return true;
}

bool ValueConversion::textEditValueToStringConversion(float value, char utf8String[256], void* userData) {
	if (userData == nullptr) {
		sprintf(utf8String, "%1.2f", value);
		return true;
	}
	valueToStringUserData* v = (valueToStringUserData*)userData;

	switch (v->precision) {
	case 0:
		sprintf(utf8String, "%1.0f", value);
		break;
	case 1:
		sprintf(utf8String, "%1.1f", value);
		break;
	case 2:
		sprintf(utf8String, "%1.2f", value);
		break;
	case 3:
		sprintf(utf8String, "%1.3f", value);
		break;
	case 4:
		sprintf(utf8String, "%1.4f", value);
		break;
	case 5:
		sprintf(utf8String, "%1.5f", value);
		break;
	default:
		sprintf(utf8String, "%1.2f", value);
		break;
	}
	sprintf(utf8String + strlen(utf8String), " ");
	sprintf(utf8String + strlen(utf8String), v->unit.c_str());
	return true;
}

double ValueConversion::getMaxEqFrequency() {
	long int temp = (long int)sampleRate;
	double maxFreq = 0.0;
	// All possible MME sampling frequencies in Windows
	switch (temp) {
	case 8000:
		maxFreq = 3600.0;
		break;
	case 9600:
		maxFreq = 4500.0;
		break;
	case 11025:
		maxFreq = 5000.0;
		break;
	case 12000:
		maxFreq = 5500.0;
		break;
	case 16000:
		maxFreq = 7000.0;
		break;
	case 22050:
		maxFreq = 10000.0;
		break;
	case 24000:
		maxFreq = 11000.0;
		break;
	case 32000:
		maxFreq = 15000.0;
		break;
	case 44100:
		maxFreq = 20000.0;
		break;
	case 48000:
		maxFreq = 22000.0;
		break;
	case 88200:
		maxFreq = 40000.0;
		break;
	case 96000:
		maxFreq = 44000.0;
		break;
	case 192000:
		maxFreq = 88000.0;
		break;
	default:
		maxFreq = 20000.0;
		break;
	}
	return maxFreq;
}

bool ValueConversion::checkEqStability(const double& b1, const double& b2) {
	// Source: Kammeyer p.77 / 78
	bool stable = false;
	if (b2 > (pow(b1, 2.0) / 4.0)) {
		if (b2 < 1.0) {
			stable = true;
		}
	}
	else if (b2 < (pow(b1, 2.0) / 4.0)) {
		if (b1 < (1.0 + b2) && b1 >(-1.0 - b2)) {
			stable = true;
		}
	}
	else if (b2 == (pow(b1, 2.0) / 4.0)) {
		if (b1 < 2.0 && b1 > -2.0) {
			stable = true;
		}
	}
	return stable;
}