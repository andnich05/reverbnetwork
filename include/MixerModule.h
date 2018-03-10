/*
* MixerModule: Mix several input signals into one
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

#ifndef MIXERMODULE_H
#define MIXERMODULE_H

#include <vector>

class MixerModule
{
public:
	MixerModule(const double& defaultGain);
	~MixerModule();

	// Mix samples of each channels, return the new sample value
	double mixInputs(double* moduleInputBuffer, double* vstInputBuffer, double& signalGeneratorSample) const;
	// Set gain of an input
	inline void setInputGain(int input, double gain) { inputGain[input] = gain; }
	// Mute or unmute an input
	inline void setInputMuted(int input, bool mute) { inputMuted[input] = mute; }
	// Solo or unsolo an input
	void setInputSoloed(int input, bool solo);

private:
	std::vector<double> inputGain; // Gain values for each input
	std::vector<bool> inputMuted; // Is an input muted?
	std::vector<bool> inputSoloed; // Is an input soloed?

	bool isAnyInputSoloed; // Is any of the inputs soloed?
};

#endif // MIXERMODULE_H
