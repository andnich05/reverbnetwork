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

#include "../include/ReverbNetworkProcessor.h"
#include "../include/reverbnetworkids.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"

#include "../include/ReverbNetworkDefines.h"
#include "../include/ReverbNetworkEnums.h"

#include "../include/BaseAPModule.h"
#include "../include/ConnectionMatrix.h"
#include "../include/ValueConversion.h"
#include "../include/PresetReadWrite.h"
#include "../include/SignalGenerator.h"



#ifdef LOGGING
#include "Logging.h"
#endif

// Performance
//#include <chrono>

namespace Steinberg {
namespace Vst {

//-----------------------------------------------------------------------------
ReverbNetworkProcessor::ReverbNetworkProcessor() {
	setControllerClass(ReverbNetworkControllerUID);

	moduleInputBuffer = new double[MAXMODULENUMBER]();
	moduleOutputBuffer = new double[MAXMODULENUMBER]();

	connectionMatrix = new ConnectionMatrix();

	ppmValues.resize(MAXMODULENUMBER, 0.0);
	ppmOldValues.resize(MAXMODULENUMBER, 0.0);
	eqStabilityValues.resize(MAXMODULENUMBER, true);
	eqStabilityOldValues.resize(MAXMODULENUMBER, true);

	preset = new PresetReadWrite();

	for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
		BaseAPModule* module = new BaseAPModule();
		apModules.push_back(module);
	}

	signalGenerator = new SignalGenerator(SignalGeneratorType::dirac);
}

ReverbNetworkProcessor::~ReverbNetworkProcessor() {
	timerUpdateController->stop();
	if (moduleInputBuffer) {
		delete[] moduleInputBuffer;
		moduleInputBuffer = nullptr;
	}
	if (moduleOutputBuffer) {
		delete[] moduleOutputBuffer;
		moduleOutputBuffer = nullptr;
	}
	for (auto&& module : apModules) {
		if (module) {
			delete module;
			module = nullptr;
		}
	}
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API ReverbNetworkProcessor::initialize(FUnknown* context)
{
	tresult result = AudioEffect::initialize (context);
	if (result == kResultTrue)
	{
		// Create I/O
		for (unsigned int i = 0; i < 1; ++i) {
			std::string temp = "VST IN 5.1 ";
			temp.append(std::to_string(i));
			addAudioInput(USTRING(temp.c_str()), SpeakerArr::k51);
		}
		for (unsigned int i = 0; i < 1; ++i) {
			std::string temp = "VST OUT 5.1 ";
			temp.append(std::to_string(i));
			addAudioOutput(USTRING(temp.c_str()), SpeakerArr::k51);
		}

		// Create MIDI input event (1 bus with 1 channel for now...)
		addEventInput(USTRING("Event In"), 1);

		// SampleRate is always 44100 here...
		ValueConversion::setSampleRate(processSetup.sampleRate);
		for (auto&& module : apModules) {
			module->setSampleRate(processSetup.sampleRate);
		}

		// Create Timer
		timerUpdateController = Timer::create(this, 500);
	}
	return result;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API ReverbNetworkProcessor::setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts)
{
	// Host wants another channel configuration
	// Create 1 input and 1 output with the desired number of channels by the host
	if (numIns >= 1 && numOuts >= 1) {
		if (SpeakerArr::getChannelCount(inputs[0]) >= 1 && SpeakerArr::getChannelCount(outputs[0]) >= 1) {
			removeAudioBusses();

			std::string temp = "VST IN ";
			temp.append(std::to_string(0));
			addAudioInput(USTRING(temp.c_str()), inputs[0]);

			temp = "VST OUT ";
			temp.append(std::to_string(0));
			addAudioOutput(USTRING(temp.c_str()), outputs[0]);

			/*std::string temp = "VST IN";
			addAudioInput(USTRING(temp.c_str()), SpeakerArr::k91);
			temp = "VST OUT";
			addAudioOutput(USTRING(temp.c_str()), SpeakerArr::k91);*/

			return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
			
		}
	}
	return kResultFalse;
}

//tresult PLUGIN_API ReverbNetworkProcessor::getBusArrangement(BusDirection dir, int32 index, SpeakerArrangement& arr) {
//
//	// Fallback solution: make every in- and output mono
//	arr = SpeakerArr::kMono;
//	return kResultOk;
//}

tresult PLUGIN_API ReverbNetworkProcessor::setActive(TBool state)
{
	ValueConversion::setSampleRate(processSetup.sampleRate);
	for (auto&& module : apModules) {
		module->setSampleRate(processSetup.sampleRate);
	}
	#ifdef LOGGING
	Logging::addToLog("PROCESSOR", "Sample rate set to " + std::to_string(processSetup.sampleRate) + " in setActive()");
	#endif

	// Check number of Channels
	SpeakerArrangement arr;
	if (getBusArrangement (kOutput, 0, arr) != kResultTrue)
		return kResultFalse;
	int32 numChannels = SpeakerArr::getChannelCount (arr);
	if (numChannels == 0)
		return kResultFalse;

	if (state) // Plugin enabled
	{
	#ifdef LOGGING
		Logging::addToLog("PROCESSOR", "Plug-in enabled");
	#endif

	}
	else // Plugin disabled => Free buffer
	{
	#ifdef LOGGING
		Logging::addToLog("PROCCESSOR", "Plug-in disabled");
	#endif
	}
	
	return AudioEffect::setActive (state);
}

tresult PLUGIN_API ReverbNetworkProcessor::setState(IBStream* state)
{
	ValueConversion::setSampleRate(processSetup.sampleRate);
	for (auto&& module : apModules) {
		module->setSampleRate(processSetup.sampleRate);
	}
	#ifdef LOGGING
	Logging::addToLog("PROCESSOR", "Sample rate set to " + std::to_string(processSetup.sampleRate) + " in setState()");
	#endif
	return preset->setParamterState(state);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API ReverbNetworkProcessor::getState(IBStream* state)
{
	#ifdef LOGGING
	Logging::addToLog("PROCESSOR", "getState() called");
	#endif
	return preset->getParamterState(state);
}

void ReverbNetworkProcessor::onTimer(Timer* timer) {
	if (ValueConversion::getSampleRate() != (processSetup.sampleRate)) {
		// Make sure that the right sample rate is used!
		ValueConversion::setSampleRate(processSetup.sampleRate);
		for (auto&& module : apModules) {
			module->setSampleRate(processSetup.sampleRate);
		}
	}

	for (unsigned int i = 0; i < eqStabilityValues.size(); ++i) {
		// Check if stability has changed in the modules
		if (eqStabilityValues[i] != eqStabilityOldValues[i]) {
			// If so prepare new message for the controller
			EqualizerStability* eqStability = new EqualizerStability();
			eqStability->moduleNumber = i;
			eqStability->isStable = eqStabilityValues[i];
			// Create VST message
			IMessage* message = allocateMessage();
			message->setMessageID("EqStability");
			// Set message content
			message->getAttributes()->setBinary(0, eqStability, sizeof(*eqStability));
			// Send the message to the controller
			sendMessage(message);
			// Delete message
			message->release();
			// Update stability values
			eqStabilityOldValues[i] = eqStabilityValues[i];
		}
	}
}


//-----------------------------------------------------------------------------
tresult PLUGIN_API ReverbNetworkProcessor::process(ProcessData& data)
{
#ifdef LOG
	FILE* pFile = fopen("E:\\logVst.txt", "a");
#endif
	// If the user changed a control in the plugin (e.g. Delay Time) => update values
	if (data.inputParameterChanges)
	{	
		// For each parameter change
		// Adobe Audition does not call the process() Function until for example 'Play' is pressed => until then the paramters are not updated
		for (int32 index = 0; index < data.inputParameterChanges->getParameterCount(); index++) {
			IParamValueQueue* queue = data.inputParameterChanges->getParameterData (index); // Get all changes in queue for the parameter (seems to be always 1)
			if (queue) {
				int32 pid = 0;
				int32 valueChangeCount = queue->getPointCount(); // seems to be always 1
				ParamValue value = 0.0;
				int32 sampleOffset = 0.0;
				pid = queue->getParameterId(); // Parameter IDs see ReverbNetworkDefines.h

				// Update preset values (important when the user closes the Editor and reopens it => the preset is used for initialization)
				if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
					preset->setNormValueByParamId(value, pid);
				}

				// Old version with connection matrix
				/*if (pid >= PARAM_MIXERINPUTSELECT_FIRST && pid <= PARAM_MIXERINPUTSELECT_LAST) {
					// Get only the last change of the value
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						uint16 moduleNumber = (pid - PARAM_MIXERINPUTSELECT_FIRST) / MAXMODULEINPUTS;	// Calculate the module number
						uint16 moduleInput = (pid - PARAM_MIXERINPUTSELECT_FIRST) % MAXMODULEINPUTS;
						value = ValueConversion::normToValueMixerInputSelect(value); // Transform normalized value to list index
						if (value == 0) { // <Not Connected> selected
							connectionMatrix->disconnectModuleInput(moduleNumber, moduleInput);
						}
						else if (value - 1 < MAXMODULENUMBER) { // Another module output as input source selected
							connectionMatrix->setModuleToModuleConnection(value - 1, moduleNumber, moduleInput);
						}
						else { // VST input as input source selected
							connectionMatrix->setVstToModuleConnection(value - 1 - MAXMODULENUMBER, moduleNumber, moduleInput);
						}
					}
				}*/

				if (pid >= PARAM_MIXERGAIN_FIRST && pid <= PARAM_MIXERGAIN_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						uint16 moduleNumber = (pid - PARAM_MIXERGAIN_FIRST) / MAXINPUTS;	// Calculate the module number
						uint16 moduleInput = (pid - PARAM_MIXERGAIN_FIRST) % MAXINPUTS;
						apModules[moduleNumber]->updateMixerGain(moduleInput, ValueConversion::normToPlainInputGain(value));
						#ifdef LOGGING
						Logging::addToLog("MIXER", "Module " + std::to_string(moduleNumber) + " - Input " + std::to_string(moduleInput) + " Gain set to " + std::to_string(ValueConversion::normToPlainInputGain(value)));
						#endif
					}
				}
				else if (pid >= PARAM_MIXERINPUTMUTED_FIRST && pid <= PARAM_MIXERINPUTMUTED_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						uint16 moduleNumber = (pid - PARAM_MIXERINPUTMUTED_FIRST) / MAXINPUTS;	// Calculate the module number
						uint16 moduleInput = (pid - PARAM_MIXERINPUTMUTED_FIRST) % MAXINPUTS;
						apModules[moduleNumber]->updateMixerMute(moduleInput, value);
						#ifdef LOGGING
						Logging::addToLog("MIXER", "Module " + std::to_string(moduleNumber) + " - Input " + std::to_string(moduleInput) + " - Mute set to " + std::to_string(value));
						#endif
					}
				}
				else if (pid >= PARAM_MIXERINPUTSOLOED_FIRST && pid <= PARAM_MIXERINPUTSOLOED_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						uint16 moduleNumber = (pid - PARAM_MIXERINPUTSOLOED_FIRST) / MAXINPUTS;	// Calculate the module number
						uint16 moduleInput = (pid - PARAM_MIXERINPUTSOLOED_FIRST) % MAXINPUTS;
						apModules[moduleNumber]->updateMixerSolo(moduleInput, value);
						#ifdef LOGGING
						Logging::addToLog("MIXER", "Module " + std::to_string(moduleNumber) + " - Input " + std::to_string(moduleInput) + " - Solo set to " + std::to_string(value));
						#endif
					}
				}
				else if (pid >= PARAM_QUANTIZERBITDEPTH_FIRST && pid <= PARAM_QUANTIZERBITDEPTH_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_QUANTIZERBITDEPTH_FIRST]->updateQuantizerQuantization(ValueConversion::normToPlainQuantization(value));
						#ifdef LOGGING
						Logging::addToLog("QUANTIZER", "Module " + std::to_string(pid - PARAM_QUANTIZERBITDEPTH_FIRST) + " - Quantization set to " + std::to_string(ValueConversion::normToPlainQuantization(value)));
						#endif
					}
				}
				else if (pid >= PARAM_QUANTIZERBYPASS_FIRST && pid <= PARAM_QUANTIZERBYPASS_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_QUANTIZERBYPASS_FIRST]->switchQuantizerBypass(value);
						#ifdef LOGGING
						Logging::addToLog("QUANTIZER", "Module " + std::to_string(pid - PARAM_QUANTIZERBYPASS_FIRST) + " - Bypass set to " + std::to_string(value));
						#endif
					}
				}
				else if (pid >= PARAM_EQFILTERTYPE_FIRST && pid <= PARAM_EQFILTERTYPE_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						value = ValueConversion::normToPlainFilterTypeSelect(value);
						eqStabilityValues[pid - PARAM_EQFILTERTYPE_FIRST] = apModules[pid - PARAM_EQFILTERTYPE_FIRST]->updateEqualizerFilterType(value);
						#ifdef LOGGING
						Logging::addToLog("EQUALIZER", "Module " + std::to_string(pid - PARAM_EQFILTERTYPE_FIRST) + " - Filter type set to " + std::to_string(value));
						#endif
					}
				}
				else if (pid >= PARAM_EQCENTERFREQ_FIRST && pid <= PARAM_EQCENTERFREQ_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						eqStabilityValues[pid - PARAM_EQCENTERFREQ_FIRST] = apModules[pid - PARAM_EQCENTERFREQ_FIRST]->updateEqualizerCenterFrequency(ValueConversion::normToPlainProcCenterFreq(value));
						#ifdef LOGGING
						Logging::addToLog("EQUALIZER", "Module " + std::to_string(pid - PARAM_EQCENTERFREQ_FIRST) + " - Center Frequency set to " + std::to_string(ValueConversion::normToPlainProcCenterFreq(value)));
						#endif
					}
				}
				else if (pid >= PARAM_EQQFACTOR_FIRST && pid <= PARAM_EQQFACTOR_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						eqStabilityValues[pid - PARAM_EQQFACTOR_FIRST] = apModules[pid - PARAM_EQQFACTOR_FIRST]->updateEqualizerQFactor(ValueConversion::normToPlainQFactor(value));
						#ifdef LOGGING
						Logging::addToLog("EQUALIZER", "Module " + std::to_string(pid - PARAM_EQQFACTOR_FIRST) + " - QFactor set to " + std::to_string(ValueConversion::normToPlainQFactor(value)));
						#endif
					}
				}
				else if (pid >= PARAM_EQGAIN_FIRST && pid <= PARAM_EQGAIN_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						eqStabilityValues[pid - PARAM_EQGAIN_FIRST] = apModules[pid - PARAM_EQGAIN_FIRST]->updateEqualizerGain(ValueConversion::normToPlainEqGain(value));
						#ifdef LOGGING
						Logging::addToLog("EQUALIZER", "Module " + std::to_string(pid - PARAM_EQGAIN_FIRST) + " - Gain set to " + std::to_string(ValueConversion::normToPlainEqGain(value)));
						#endif
					}
				}
				else if (pid >= PARAM_EQCOEFFICIENTA0_FIRST && pid <= PARAM_EQCOEFFICIENTA0_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						eqStabilityValues[pid - PARAM_EQCOEFFICIENTA0_FIRST] = apModules[pid - PARAM_EQCOEFFICIENTA0_FIRST]->updateEqualizerCoefficients(ValueConversion::normToPlainEqCoefficients(value), pid);
						#ifdef LOGGING
						Logging::addToLog("EQUALIZER", "Module " + std::to_string(pid - PARAM_EQCOEFFICIENTA0_FIRST) + " - a0 set to " + std::to_string(ValueConversion::normToPlainEqCoefficients(value)));
						#endif
					}
				}
				else if (pid >= PARAM_EQCOEFFICIENTA1_FIRST && pid <= PARAM_EQCOEFFICIENTA1_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						eqStabilityValues[pid - PARAM_EQCOEFFICIENTA1_FIRST] = apModules[pid - PARAM_EQCOEFFICIENTA1_FIRST]->updateEqualizerCoefficients(ValueConversion::normToPlainEqCoefficients(value), pid);
						#ifdef LOGGING
						Logging::addToLog("EQUALIZER", "Module " + std::to_string(pid - PARAM_EQCOEFFICIENTA1_FIRST) + " - a1 set to " + std::to_string(ValueConversion::normToPlainEqCoefficients(value)));
						#endif
					}
				}
				else if (pid >= PARAM_EQCOEFFICIENTA2_FIRST && pid <= PARAM_EQCOEFFICIENTA2_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						eqStabilityValues[pid - PARAM_EQCOEFFICIENTA2_FIRST] = apModules[pid - PARAM_EQCOEFFICIENTA2_FIRST]->updateEqualizerCoefficients(ValueConversion::normToPlainEqCoefficients(value), pid);
						#ifdef LOGGING
						Logging::addToLog("EQUALIZER", "Module " + std::to_string(pid - PARAM_EQCOEFFICIENTA2_FIRST) + " - a2 set to " + std::to_string(ValueConversion::normToPlainEqCoefficients(value)));
						#endif
					}
				}
				else if (pid >= PARAM_EQCOEFFICIENTB1_FIRST && pid <= PARAM_EQCOEFFICIENTB1_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						eqStabilityValues[pid - PARAM_EQCOEFFICIENTB1_FIRST] = apModules[pid - PARAM_EQCOEFFICIENTB1_FIRST]->updateEqualizerCoefficients(ValueConversion::normToPlainEqCoefficients(value), pid);
						#ifdef LOGGING
						Logging::addToLog("EQUALIZER", "Module " + std::to_string(pid - PARAM_EQCOEFFICIENTB1_FIRST) + " - b1 set to " + std::to_string(ValueConversion::normToPlainEqCoefficients(value)));
						#endif
					}
				}
				else if (pid >= PARAM_EQCOEFFICIENTB2_FIRST && pid <= PARAM_EQCOEFFICIENTB2_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						eqStabilityValues[pid - PARAM_EQCOEFFICIENTB2_FIRST] = apModules[pid - PARAM_EQCOEFFICIENTB2_FIRST]->updateEqualizerCoefficients(ValueConversion::normToPlainEqCoefficients(value), pid);
						#ifdef LOGGING
						Logging::addToLog("EQUALIZER", "Module " + std::to_string(pid - PARAM_EQCOEFFICIENTB2_FIRST) + " - b2 set to " + std::to_string(ValueConversion::normToPlainEqCoefficients(value)));
						#endif
					}
				}
				else if (pid >= PARAM_EQLIMITER_FIRST && pid <= PARAM_EQLIMITER_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_EQLIMITER_FIRST]->updateEqualizerLimiter(value);
						#ifdef LOGGING
						Logging::addToLog("EQUALIZER", "Module " + std::to_string(pid - PARAM_EQLIMITER_FIRST) + " - Limiter set to " + std::to_string(value));
						#endif
					}
				}
				else if (pid >= PARAM_EQBYPASS_FIRST && pid <= PARAM_EQBYPASS_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_EQBYPASS_FIRST]->switchEqualizerBypass(value);
						#ifdef LOGGING
						Logging::addToLog("EQUALIZER", "Module " + std::to_string(pid - PARAM_EQBYPASS_FIRST) + " - Bypass set to " + std::to_string(value));
						#endif
					}
				}
				else if (pid >= PARAM_ALLPASSDELAY_FIRST && pid <= PARAM_ALLPASSDELAY_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_ALLPASSDELAY_FIRST]->updateAllpassDelay(ValueConversion::normToPlainDelay(value));
						#ifdef LOGGING
						Logging::addToLog("ALLPASS", "Module " + std::to_string(pid - PARAM_ALLPASSDELAY_FIRST) + " - Delay set to " + std::to_string(ValueConversion::normToPlainDelay(value)));
						#endif
					}
				}
				else if (pid >= PARAM_ALLPASSDECAY_FIRST && pid <= PARAM_ALLPASSDECAY_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_ALLPASSDECAY_FIRST]->updateAllpassDecay(ValueConversion::normToPlainDecay(value));
						#ifdef LOGGING
						Logging::addToLog("ALLPASS", "Module " + std::to_string(pid - PARAM_ALLPASSDECAY_FIRST) + " - Decay set to " + std::to_string(ValueConversion::normToPlainDecay(value)));
						#endif
					}
				}
				else if (pid >= PARAM_ALLPASSDIFFKSIGN_FIRST && pid <= PARAM_ALLPASSDIFFKSIGN_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_ALLPASSDIFFKSIGN_FIRST]->updateAllpassDiffKSign(value != 0.0);
						#ifdef LOGGING
						Logging::addToLog("ALLPASS", "Module " + std::to_string(pid - PARAM_ALLPASSDIFFKSIGN_FIRST) + " - DiffK sign is negative: " + std::to_string(value));
						#endif
					}
				}
				else if (pid >= PARAM_ALLPASSMODENABLED_FIRST && pid <= PARAM_ALLPASSMODENABLED_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_ALLPASSMODENABLED_FIRST]->updateAllpassModulationEnabled(value != 0.0);
						#ifdef LOGGING
						Logging::addToLog("ALLPASS", "Module " + std::to_string(pid - PARAM_ALLPASSMODENABLED_FIRST) + " - Modulation enabled set to " + std::to_string(value));
						#endif
					}
				}
				else if (pid >= PARAM_ALLPASSMODSIGNALTYPE_FIRST && pid <= PARAM_ALLPASSMODSIGNALTYPE_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_ALLPASSMODSIGNALTYPE_FIRST]->updateAllpassModulationSignalType(ValueConversion::normToPlainModSignalType(value));
						#ifdef LOGGING
						Logging::addToLog("ALLPASS", "Module " + std::to_string(pid - PARAM_ALLPASSMODSIGNALTYPE_FIRST) + " - Modulation signal type set to " + std::to_string(ValueConversion::normToPlainModSignalType(value)));
						#endif
					}
				}
				else if (pid >= PARAM_ALLPASSMODEXCURSION_FIRST && pid <= PARAM_ALLPASSMODEXCURSION_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_ALLPASSMODEXCURSION_FIRST]->updateAllpassModulationExcursion(ValueConversion::normToPlainModExcursion(value));
						#ifdef LOGGING
						Logging::addToLog("ALLPASS", "Module " + std::to_string(pid - PARAM_ALLPASSMODEXCURSION_FIRST) + " - Modulation excursion set to " + std::to_string(ValueConversion::normToPlainModExcursion(value)));
						#endif
					}
				}
				else if (pid >= PARAM_ALLPASSMODRATE_FIRST && pid <= PARAM_ALLPASSMODRATE_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_ALLPASSMODRATE_FIRST]->updateAllpassModulationRate(ValueConversion::normToPlainModRate(value));
						#ifdef LOGGING
						Logging::addToLog("ALLPASS", "Module " + std::to_string(pid - PARAM_ALLPASSMODRATE_FIRST) + " - Modulation rate set to " + std::to_string(ValueConversion::normToPlainModRate(value)));
						#endif
					}
				}
				else if (pid >= PARAM_ALLPASSBYPASS_FIRST && pid <= PARAM_ALLPASSBYPASS_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_ALLPASSBYPASS_FIRST]->switchAllpassBypass(value);
						#ifdef LOGGING
						Logging::addToLog("ALLPASS", "Module " + std::to_string(pid - PARAM_ALLPASSBYPASS_FIRST) + " - Bypass set to " + std::to_string(value));
						#endif
					}
				}
				else if (pid >= PARAM_OUTGAIN_FIRST && pid <= PARAM_OUTGAIN_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_OUTGAIN_FIRST]->updateOutputGain(ValueConversion::normToPlainOutputGain(value));
						#ifdef LOGGING
						Logging::addToLog("OUTPUT", "Module " + std::to_string(pid - PARAM_OUTGAIN_FIRST) + " - Gain set to " + std::to_string(ValueConversion::normToPlainOutputGain(value)));
						#endif
					}
				}
				else if (pid >= PARAM_OUTLIMITER_FIRST && pid <= PARAM_OUTLIMITER_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_OUTLIMITER_FIRST]->updateOutputLimiter(value);
						#ifdef LOGGING
						Logging::addToLog("OUTPUT", "Module " + std::to_string(pid - PARAM_OUTLIMITER_FIRST) + " - Limiter set to " + std::to_string(value));
						#endif
					}
				}
				else if (pid >= PARAM_OUTBYPASS_FIRST && pid <= PARAM_OUTBYPASS_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_OUTBYPASS_FIRST]->switchOutputBypass(value);
						#ifdef LOGGING
						Logging::addToLog("OUTPUT", "Module " + std::to_string(pid - PARAM_OUTBYPASS_FIRST) + " - Bypass set to " + std::to_string(value));
						#endif
					}
				}
				else if (pid == PARAM_SIGNALGENERATOR_SIGNALTYPE) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						if ((int)ValueConversion::normToPlainSignalType(value) < (int)SignalGeneratorType::numberOfSignalGeneratorTypes) {
							signalGenerator->setSignalType(static_cast<SignalGeneratorType>((int)ValueConversion::normToPlainSignalType(value)));
							#ifdef LOGGING
							Logging::addToLog("SIGNALGEN", "Signal Type set to " + std::to_string((int)ValueConversion::normToPlainSignalType(value)));
							#endif
						}
					}
				}
				else if (pid == PARAM_SIGNALGENERATOR_AMPLITUDE) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						signalGenerator->setGain(ValueConversion::normToPlainSignalAmplitude(value));
						#ifdef LOGGING
						Logging::addToLog("SIGNALGEN", "Gain set to " + std::to_string(ValueConversion::normToPlainSignalAmplitude(value)));
						#endif
					}
				}
				else if (pid == PARAM_SIGNALGENERATOR_WIDTH) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						signalGenerator->setWidth(ValueConversion::normToPlainSignalWidth(value));
						#ifdef LOGGING
						Logging::addToLog("SIGNALGEN", "Width set to " + std::to_string(ValueConversion::normToPlainSignalWidth(value)));
						#endif
					}
				} 
				else if (pid == PARAM_SIGNALGENERATOR_TIME) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						signalGenerator->setAutoTime(ValueConversion::normToPlainSignalTime(value));
						#ifdef LOGGING
						Logging::addToLog("SIGNALGEN", "Auto Time set to " + std::to_string(ValueConversion::normToPlainSignalTime(value)));
						#endif
					}
				} 
				else if (pid == PARAM_SIGNALGENERATOR_AUTOFIREENABLED) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						signalGenerator->setAutoFireEnabled(value != 0.0);
						#ifdef LOGGING
						Logging::addToLog("SIGNALGEN", "Auto Fire enabled set to " + std::to_string(value));
						#endif
					}
				}
				else if (pid == PARAM_SIGNALGENERATOR_FIRE) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						signalGenerator->setFire(value != 0.0);
						#ifdef LOGGING
						Logging::addToLog("SIGNALGEN", "Fire!");
						#endif
					}
				}
				//...
				else if (pid >= PARAM_GENERALVSTOUTPUTSELECT_FIRST && pid <= PARAM_GENERALVSTOUTPUTSELECT_LAST) {
					// Get only the last change of the value
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						int index = (int)(ValueConversion::normToPlainMixerInputSelect(value));
						if (index == 0) { // <Not Connected> selected
							connectionMatrix->disconnectVstOutput(pid - PARAM_GENERALVSTOUTPUTSELECT_FIRST);
							#ifdef LOGGING
							Logging::addToLog("VST OUTPUT", "Disconnect VST Output " + std::to_string(pid - PARAM_GENERALVSTOUTPUTSELECT_FIRST));
							#endif
						}
						else if (index < MAXMODULENUMBER + 1) { // Another module output as input source selected
							connectionMatrix->setModuleToVstConnection(index - 1, pid - PARAM_GENERALVSTOUTPUTSELECT_FIRST);
							#ifdef LOGGING
							Logging::addToLog("VST OUTPUT", "Connect Module " + std::to_string(index - 1) + " to VST Output " + std::to_string(pid - PARAM_GENERALVSTOUTPUTSELECT_FIRST));
							#endif
						}
						else { // VST input as input source selected
							connectionMatrix->setVstToVstConnection(index - 1 - MAXMODULENUMBER, pid - PARAM_GENERALVSTOUTPUTSELECT_FIRST);
							#ifdef LOGGING
							Logging::addToLog("VST OUTPUT", "Connect VST Input " + std::to_string(index - 1 - MAXMODULENUMBER) + " to VST Output " + std::to_string(pid - PARAM_GENERALVSTOUTPUTSELECT_FIRST));
							#endif
							
						}
					}
				}
			}
		}
	}

	// Process input events (e.g. MIDI)
	// Doesn't work in all VST Hosts
	if (data.inputEvents) {
		if (data.inputEvents->getEventCount() > 0) {
			Event inputEvent = {};
			data.inputEvents->getEvent(0, inputEvent);
			if (inputEvent.type == Event::kNoteOnEvent) {
				// Fire Signal Generator if a note is on
				signalGenerator->setFire(true);
				#ifdef LOGGING
				Logging::addToLog("MIDI", "Fire!");
				#endif
			}
		}
	}

	// Process the audio samples
	if (data.numSamples > 0 && data.numInputs != 0 && data.numOutputs != 0) {
		// Reset PPM values
		std::fill(ppmValues.begin(), ppmValues.end(), 0.0);

		uint32 numberOfSamples = data.numSamples;

		// Sample format is -1.0 to +1.0
		float* inputSamples[MAXVSTINPUTS];
		float* outputSamples[MAXVSTOUTPUTS];

		//----------------------

		// Sample interval
		//t0 = std::chrono::high_resolution_clock::now();
		// Get the input buffers
		for (int32 channel = 0; channel < data.inputs[0].numChannels; ++channel) {
			inputSamples[channel] = data.inputs[0].channelBuffers32[channel];
		}
		// Get the output buffers
		for (int32 channel = 0; channel < data.outputs[0].numChannels; ++channel) {
			outputSamples[channel] = data.outputs[0].channelBuffers32[channel];
		}

		// Get the connection matrix
		//const std::vector<std::vector<short>>& moduleInputConnections = connectionMatrix->getModuleInputConnections();
		const std::vector<short>& vstOutputConnections = connectionMatrix->getVstOutputConnections();

		// Vector with all samples for all inputs which are connected to a module input
		double vstInputBuffer[MAXVSTINPUTS];
		double signalGeneratorSample = 0.0;

		for (unsigned long sample = 0; sample < numberOfSamples; ++sample) {
			signalGeneratorSample = signalGenerator->generateSample();
			// Module input processing
			for (auto module = 0; module < MAXMODULENUMBER; ++module) {
				//vstInputBuffer.clear();

				for (int32 channel = 0; channel < data.inputs[0].numChannels; ++channel) {
					vstInputBuffer[channel] = (double)inputSamples[channel][sample]; // Crash!!!
				}

				// Process the vector and write the output sample into the correct module output buffer
				moduleOutputBuffer[module] = apModules[module]->processSamples(moduleInputBuffer, vstInputBuffer, signalGeneratorSample);
				// Update PPM values
				if (moduleOutputBuffer[module] > ppmValues[module]) {
					ppmValues[module] = moduleOutputBuffer[module];
				}
			}

			// VST output processing
			for (auto vstOutput = 0; vstOutput < data.outputs[0].numChannels; ++vstOutput) {
				if (vstOutputConnections[vstOutput] != -1) {
					if (vstOutputConnections[vstOutput] < MAXMODULENUMBER) {
						// VST output is connected to a module's output => take sample from the module output buffer
						outputSamples[vstOutput][sample] = (moduleOutputBuffer[vstOutputConnections[vstOutput]]);
							
					}
					else if (vstOutputConnections[vstOutput] - MAXMODULENUMBER < data.inputs[0].numChannels) {
						// VST output is connected directly to VST input => take sample from the VST input
						outputSamples[vstOutput][sample] = inputSamples[vstOutputConnections[vstOutput] - MAXMODULENUMBER][sample];
					}
					else {
						// Not used VST input is connected to a used VST output => sample value is zero
						outputSamples[vstOutput][sample] = 0.0;
					}
				}
				else {
					// Output isn't connected => sample value is zero
					outputSamples[vstOutput][sample] = 0.0;
				}
			}

			// Swap input and output buffers
			double* temp = moduleInputBuffer;
			moduleInputBuffer = moduleOutputBuffer;
			moduleOutputBuffer = temp;

		}
		

		//t1 = std::chrono::high_resolution_clock::now();
		
		//---3) Write outputs parameter changes-----------
		IParameterChanges* paramChanges = data.outputParameterChanges;
		// a new value of VuMeter will be send to the host 
		// (the host will send it back in sync to our controller for updating our editor)
		// The function "setParamNormalized()" will be called in the Controller
		if (paramChanges) {
			for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
				// Check if the values have changed since last time
				if (ppmValues[i] != ppmOldValues[i]) {
					int32 index = 0;
					// Add the changes to the output parameter queue
					IParamValueQueue* paramQueue = paramChanges->addParameterData(PARAM_PPMUPDATE_FIRST + i, index);
					if (paramQueue) {
						int32 index2 = 0;
						paramQueue->addPoint(0, ppmValues[i], index2);
						ppmOldValues[i] = ppmValues[i];
						
					}
				}
			}
		}
	}

	return kResultTrue;
}

}} // namespaces
