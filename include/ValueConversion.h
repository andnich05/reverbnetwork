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

#ifndef VALUECONVERSION_H
#define VALUECONVERSION_H

#include "../vstgui4/vstgui/lib/controls/cparamdisplay.h"

class ValueConversion
{
public:
	ValueConversion();

	// Sample rate of the host
	inline static void setSampleRate(const double s) { sampleRate = s; }
	inline static double getSampleRate() { return sampleRate; }

	//---Value conversion functions, convert between normalized values (0.0 to 1.0) and plain values

	// Mixer
	static double normToPlainMixerInputSelect(const double& normValue);
	static double plainToNormMixerInputSelect(const double& plainValue);
	static double normToPlainInputGain(const double& normValue);
	static double plainToNormInputGain(const double& plainValue);

	// Quantizer
	static double normToPlainQuantization(const double& normValue);
	static double plainToNormQuantization(const double& plainValue);

	// Equalizer
	static double normToPlainFilterTypeSelect(const double& normValue);
	static double plainToNormFilterTypeSelect(const double& plainValue);
	static double normToPlainProcCenterFreq(const double& normValue);
	static double plainToNormProcCenterFreq(const double& plainValue);
	static double normToPlainQFactor(const double& normValue);
	static double plainToNormQFactor(const double& plainValue);
	static double normToPlainEqGain(const double& normValue);
	static double plainToNormEqGain(const double& plainValue);
	static double normToPlainEqCoefficients(const double& normValue);
	static double plainToNormEqCoefficients(const double& plainValue);

	// Allpass
	static double normToPlainDelay(const double& normValue);
	static double plainToNormDelay(const double& plainValue);
	static double delayMillisecondsToSamples(const double& delaySeconds);
	static double delaySamplesToMilliseconds(const double& delaySamples);
	static double calculateDiffK(const double& delayInMs, const double& decayInMs);
	static double diffKToDecay(const double& diffK, const double& delayInMs);
	static double normToPlainDecay(const double& normValue);
	static double plainToNormDecay(const double& plainValue);
	static double normToPlainModSignalType(const double& normValue);
	static double plainToNormModSignalType(const double& plainValue);
	static double normToPlainModExcursion(const double& normValue);
	static double plainToNormModExcursion(const double& plainValue);
	static double normToPlainModRate(const double& normValue);
	static double plainToNormModRate(const double& plainValue);

	// Output
	static double normToPlainOutputGain(const double& normValue);
	static double plainToNormOutputGain(const double& plainValue);

	// Signal generator
	static double normToPlainSignalType(const double& normValue);
	static double plainToNormSignalType(const double& plainValue);
	static double normToPlainSignalAmplitude(const double& normValue);
	static double plainToNormSignalAmplitude(const double& plainValue);
	static double normToPlainSignalWidth(const double& normValue);
	static double plainToNormSignalWidth(const double& plainValue);
	static double normToPlainSignalTime(const double& normValue);
	static double plainToNormSignalTime(const double& plainValue);

	// Log conversion
	static double linearToLog(const double& linearValue);
	static double logToLinear(const double& logValue);

	// This conversion is performed when getValue() is called on a textEdit; converts to float with atof()
	static bool textEditStringToValueConversion(const char* txt, float& result, void* userData);
	// This conversion is performed when the user does some input in the textEdit; converts the string to float with sprintf(%f...)
	// userData contains the precision for the value to be displayed
	static bool textEditValueToStringConversion(float value, char utf8String[256], VSTGUI::CParamDisplay* display);

	// Get the maximum allowed frequency for the equalizer (is smaller than sampleRate / 2)
	static double getMaxEqFrequency();

	// Check equalizer stability
	static bool checkEqStability(const double& b1, const double& b2);

private:
	static double sampleRate;

};

#endif // VALUECONVERSION_H