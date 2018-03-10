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
	inline static void setSampleRate(double s) { sampleRate = s; }
	inline static double getSampleRate() { return sampleRate; }

	//---Value conversion functions, convert between normalized values (0.0 to 1.0) and plain values

	// Mixer
	static double normToPlainMixerInputSelect(double normValue);
	static double plainToNormMixerInputSelect(double plainValue);
	static double normToPlainInputGain(double normValue);
	static double plainToNormInputGain(double plainValue);

	// Quantizer
	static double normToPlainQuantization(double normValue);
	static double plainToNormQuantization(double plainValue);

	// Equalizer
	static double normToPlainFilterTypeSelect(double normValue);
	static double plainToNormFilterTypeSelect(double plainValue);
	static double normToPlainProcCenterFreq(double normValue);
	static double plainToNormProcCenterFreq(double plainValue);
	static double normToPlainQFactor(double normValue);
	static double plainToNormQFactor(double plainValue);
	static double normToPlainEqGain(double normValue);
	static double plainToNormEqGain(double plainValue);
	static double normToPlainEqCoefficients(double normValue);
	static double plainToNormEqCoefficients(double plainValue);

	// Allpass
	static double normToPlainDelay(double normValue);
	static double plainToNormDelay(double plainValue);
	static double delayMillisecondsToSamples(double delaySeconds);
	static double delaySamplesToMilliseconds(double delaySamples);
	static double calculateDiffK(double delayInMs, double decayInMs);
	static double diffKToDecay(double diffK, double delayInMs);
	static double normToPlainDecay(double normValue);
	static double plainToNormDecay(double plainValue);
	static double normToPlainModSignalType(double normValue);
	static double plainToNormModSignalType(double plainValue);
	static double normToPlainModExcursion(double normValue);
	static double plainToNormModExcursion(double plainValue);
	static double normToPlainModRate(double normValue);
	static double plainToNormModRate(double plainValue);

	// Output
	static double normToPlainOutputGain(double normValue);
	static double plainToNormOutputGain(double plainValue);

	// Signal generator
	static double normToPlainSignalType(double normValue);
	static double plainToNormSignalType(double plainValue);
	static double normToPlainSignalAmplitude(double normValue);
	static double plainToNormSignalAmplitude(double plainValue);
	static double normToPlainSignalWidth(double normValue);
	static double plainToNormSignalWidth(double plainValue);
	static double normToPlainSignalTime(double normValue);
	static double plainToNormSignalTime(double plainValue);

	// Log conversion
	static double linearToLog(double linearValue);
	static double logToLinear(double logValue);

	// This conversion is performed when getValue() is called on a textEdit; converts to float with atof()
	static bool textEditStringToValueConversion(const char* txt, float& result, void* userData);
	// This conversion is performed when the user does some input in the textEdit; converts the string to float with sprintf(%f...)
	// userData contains the precision for the value to be displayed
	static bool textEditValueToStringConversion(float value, char utf8String[256], VSTGUI::CParamDisplay* display);

	// Get the maximum allowed frequency for the equalizer (is smaller than sampleRate / 2)
	static double getMaxEqFrequency();

	// Check equalizer stability
	static bool checkEqStability(double b1, double b2);

private:
	static double sampleRate;

};

#endif // VALUECONVERSION_H
