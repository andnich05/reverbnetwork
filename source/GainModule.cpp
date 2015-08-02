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