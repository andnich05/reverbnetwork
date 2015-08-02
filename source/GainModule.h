/*
* GainModule: Gain processing with limiter
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

#ifndef GAINMODULE_H
#define GAINMODULE_H

class GainModule
{
public:
	GainModule(double gain);
	~GainModule();

	// Set output gain as a factor
	inline void setGain(const double& g) { gain = g; }
	// Process sample by reference
	void processSample (double& sample) const;

private:
	// Gain as a factor
	double gain;
};

#endif // GAINMODULE_H