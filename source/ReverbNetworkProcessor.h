/*
* ReverbNetworkProcessor: Vst processor (signal processing)
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

#ifndef REVERBNETWORKPROCESSOR_H
#define REVERBNETWORKPROCESSOR_H

#include "public.sdk/source/vst/vstaudioeffect.h"
#include <vector>

#include "base/source/timer.h" // ITimerCallback

class BaseAPModule;
class ConnectionMatrix;
class PresetReadWrite;
class SignalGenerator;

namespace Steinberg {
namespace Vst {

//-----------------------------------------------------------------------------
class ReverbNetworkProcessor : public AudioEffect, public ITimerCallback
{
public:
	ReverbNetworkProcessor();
	~ReverbNetworkProcessor();
	
	tresult PLUGIN_API initialize (FUnknown* context);
	// Set input arrangement
	tresult PLUGIN_API setBusArrangements (SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts);
	//tresult PLUGIN_API getBusArrangement(BusDirection dir, int32 index, SpeakerArrangement& arr);

	// Enable or disable the plugin
	tresult PLUGIN_API setActive (TBool state);
	// Set Vst preset
	tresult PLUGIN_API setState(IBStream* state);
	// Get Vst preset
	tresult PLUGIN_API getState(IBStream* state);
	// Contains parameter changes and the whole procssing part incl. in an out buffers
	tresult PLUGIN_API process (ProcessData& data);
	
	static FUnknown* createInstance(void*) { return (IAudioProcessor*)new ReverbNetworkProcessor(); }

private:
	double* moduleInputBuffer; // Input buffer for all modules
	double* moduleOutputBuffer; // Ouput buffer for all modules
	std::vector<BaseAPModule*> apModules; // Contains the modules

	ConnectionMatrix* connectionMatrix; // Connection matrix only used for Vst output connections (for now)

	// Values to send to the controller/editor
	std::vector<double> ppmValues;
	std::vector<double> ppmOldValues;
	std::vector<bool> eqStabilityValues;
	std::vector<bool> eqStabilityOldValues;

	PresetReadWrite* preset; // Vst preset object

	// From ITimerCallback (is called by the timer)
	// What to do when timer fires
	virtual void onTimer(Timer* timer);
	Timer* timerUpdateController; // Timer object

	SignalGenerator* signalGenerator; // Signal generator object
};



}} // namespaces

#endif // REVERBNETWORKPROCESSOR_H
