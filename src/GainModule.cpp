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

#include "../include/GainModule.h"
#include "../include/ReverbNetworkDefines.h"

const double maxValue = 1.0e100;
const double minValue = -1.0e100;

GainModule::GainModule(double gain) 
	: gain(gain)
	, limit(true) {

}

GainModule::~GainModule() {

}

void GainModule::processSample(double& sample) const {
	sample *= gain;

	if (limit) {
		if (sample > 1.0) {
			sample = 1.0;
		}
		else if (sample < -1.0) {
			sample = -1.0;
		}
	}
	else {
		if (sample > maxValue) {
			sample = maxValue;
		}
		else if (sample < minValue) {
			sample = minValue;
		}
	}
}
