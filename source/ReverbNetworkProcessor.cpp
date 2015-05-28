//-----------------------------------------------------------------------------
// Project     : VST SDK
// Version     : 3.6.0
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/adelay/source/adelayprocessor.cpp
// Created by  : Steinberg, 06/2009
// Description : 
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2013, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation 
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this 
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "ReverbNetworkProcessor.h"
#include "reverbnetworkids.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include <algorithm>
#include <string>

#include "ReverbNetworkDefines.h"
#include "BaseAPModule.h"
#include "ConnectionMatrix.h"
#include "ValueConversion.h"
#include "PresetReadWrite.h"

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

	preset = new PresetReadWrite();
}

ReverbNetworkProcessor::~ReverbNetworkProcessor() {
	if (moduleInputBuffer) {
		delete[] moduleInputBuffer;
		moduleInputBuffer = nullptr;
	}
	if (moduleOutputBuffer) {
		delete[] moduleOutputBuffer;
		moduleOutputBuffer = nullptr;
	}
	/*if (connectionMatrix) {
		delete connectionMatrix;
		connectionMatrix = nullptr;
	}*/
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API ReverbNetworkProcessor::initialize(FUnknown* context)
{
	tresult result = AudioEffect::initialize (context);
	if (result == kResultTrue)
	{
		for (uint16 i = 0; i < MAXVSTINPUTS; ++i) {
			addAudioInput(STR16("Audio In Mono"), SpeakerArr::kMono);
		}
		for (uint16 i = 0; i < MAXVSTOUTPUTS; ++i) {
			addAudioOutput(STR16("Audio Out Mono"), SpeakerArr::kMono);
		}
		
		
		/*for (uint32 i = 0; i < 1; ++i) {
			std::shared_ptr<SchroederAllpass> allpass(new SchroederAllpass(44100, 44100, 0.4));
			allpasses.push_back(allpass);
		}*/

		for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
			std::shared_ptr<BaseAPModule> module(new BaseAPModule(processSetup.sampleRate));
			apModules.push_back(module);
		}

		ValueConversion::setSampleRate(processSetup.sampleRate);

	/*	FILE* pFile = fopen("E:\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", std::to_string(processSetup.symbolicSampleSize).c_str());
		fclose(pFile);*/

		/*connectionMatrix->setVstToModuleConnection(0, 0, 0);
		connectionMatrix->setVstToModuleConnection(0, 1, 0);
		connectionMatrix->setModuleToVstConnection(0, 0);
		connectionMatrix->setModuleToVstConnection(1, 1);*/
	}
	return result;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API ReverbNetworkProcessor::setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts)
{
	// Minimum one input and one output
	if (numIns >= 1 && numOuts >= 1) {
		if (SpeakerArr::getChannelCount(inputs[0]) == 1 && SpeakerArr::getChannelCount(outputs[0]) == 1) {
			return kResultTrue;
		}
	}
	return kResultFalse;
}

// User enables or disables the plugin
tresult PLUGIN_API ReverbNetworkProcessor::setActive(TBool state)
{
	// Check number of Channels
	SpeakerArrangement arr;
	if (getBusArrangement (kOutput, 0, arr) != kResultTrue)
		return kResultFalse;
	int32 numChannels = SpeakerArr::getChannelCount (arr);
	if (numChannels == 0)
		return kResultFalse;

	if (state) // Plugin enabled
	{
		/*for (uint32 i = 0; i < allpasses.size(); ++i) {
			allpasses[i]->createBuffers();
		}*/

		// Has already been done when plugin was marked as disabled, but just in case...
		/*if (allpasses.size() > 0) {
			allpasses.clear();
		}

		for (uint32 i = 0; i < 200; ++i) {
			std::shared_ptr<SchroederAllpass> allpass(new SchroederAllpass(44100, 44100, 0.4));
			allpasses.push_back(allpass);
		}*/

		/*
		// Initialize buffer (don't use 'malloc', it's C-Style, use 'new')
		// buffer = (float**)malloc (numChannels * sizeof (float*));
		
		if (inputBuffer) {
			delete[] inputBuffer;
			inputBuffer = nullptr;
		}
		if (outputBuffer) {
			delete[] outputBuffer;
			outputBuffer = nullptr;
		}

		inputBuffer = new double[(size_t)(processSetup.sampleRate)]();
		outputBuffer = new double[(size_t)(processSetup.sampleRate)]();

		// 44100*32bit+0.5 => 1 second of delay (why +0.5?)
		// size_t size = (size_t)(processSetup.sampleRate * sizeof (float) + 0.5);
		for (int32 channel = 0; channel < numChannels; channel++)
		{
			// buffer[channel] = (float*)malloc (size);	// 1 second delay max
			// memset (buffer[channel], 0, size);
		}
		//bufferPos = 0;
		*/
	}
	else // Plugin disabled => Free buffer
	{
		/*for (uint32 i = 0; i < allpasses.size(); ++i) {
			allpasses[i]->freeBuffers();
		}*/
		//if (allpasses.size() > 0) {
		//	allpasses.clear(); // Smart-Pointers => 'delete' isn't necessary
		//}

		/*
		if (inputBuffer) {
			delete[] inputBuffer;
			inputBuffer = nullptr;
		}
		if (outputBuffer) {
			delete[] outputBuffer;
			outputBuffer = nullptr;
		}

		//if (buffer)
		{
			for (int32 channel = 0; channel < numChannels; channel++)
			{
				//free (buffer[channel]);
			}
			//free (buffer);
			//buffer = 0;
		}
		*/
	}
	return AudioEffect::setActive (state);
}

// Log output for debugging
//#define LOG

tresult PLUGIN_API ReverbNetworkProcessor::setState(IBStream* state)
{
	return preset->setParamterState(state);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API ReverbNetworkProcessor::getState(IBStream* state)
{
	return preset->getParamterState(state);
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
		// Hier nix anlegen weil totale Verschwendung (wird aufgerufen auch wenn nix geändert wurde)
		
		// Number of changed parameters in the parameter list
		//int32 paramChangeCount = data.inputParameterChanges->getParameterCount ();
		// For each parameter change
		for (int32 index = 0; index < data.inputParameterChanges->getParameterCount(); index++)
		{
			IParamValueQueue* queue = data.inputParameterChanges->getParameterData (index);
			if (queue) {
				int32 pid = 0;
				int32 valueChangeCount = queue->getPointCount();
				ParamValue value = 0.0;
				int32 sampleOffset = 0.0;
				pid = queue->getParameterId();

				// Update preset values...
				if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
					preset->setNormValueByParamId(value, pid);
				}

				if (pid >= PARAM_MIXERINPUTSELECT_FIRST && pid <= PARAM_MIXERINPUTSELECT_LAST) {
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
				}
				else if (pid >= PARAM_MIXERGAIN_FIRST && pid <= PARAM_MIXERGAIN_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						uint16 moduleNumber = (pid - PARAM_MIXERGAIN_FIRST) / MAXMODULEINPUTS;	// Calculate the module number
						uint16 moduleInput = (pid - PARAM_MIXERINPUTSELECT_FIRST) % MAXMODULEINPUTS;
						apModules[moduleNumber]->updateMixerGain(moduleInput, ValueConversion::normToValueGain(value));
					}
				}
				else if (pid >= PARAM_MIXERBYPASS_FIRST && pid <= PARAM_MIXERBYPASS_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_MIXERBYPASS_FIRST]->switchMixerBypass(value);
					}
				}
				else if (pid >= PARAM_QUANTIZERBITDEPTH_FIRST && pid <= PARAM_QUANTIZERBITDEPTH_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_QUANTIZERBITDEPTH_FIRST]->updateQuantizerQuantization(ValueConversion::normToValueQuantization(value));
					}
				}
				else if (pid >= PARAM_QUANTIZERBYPASS_FIRST && pid <= PARAM_QUANTIZERBYPASS_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_QUANTIZERBYPASS_FIRST]->switchQuantizerBypass(value);
					}
				}
				else if (pid >= PARAM_EQFILTERTYPE_FIRST && pid <= PARAM_EQFILTERTYPE_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						value = ValueConversion::normToValueFilterTypeSelect(value);
						apModules[pid - PARAM_EQFILTERTYPE_FIRST]->updateEqualizerFilterType(value);
					}
				}
				else if (pid >= PARAM_EQCENTERFREQ_FIRST && pid <= PARAM_EQCENTERFREQ_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_EQCENTERFREQ_FIRST]->updateEqualizerCenterFrequency(ValueConversion::normToValueCenterFreq(value));
					}
				}
				else if (pid >= PARAM_EQQFACTOR_FIRST && pid <= PARAM_EQQFACTOR_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_EQQFACTOR_FIRST]->updateEqualizerQFactor(ValueConversion::normToValueQFactor(value));
					}
				}
				else if (pid >= PARAM_EQGAIN_FIRST && pid <= PARAM_EQGAIN_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_EQGAIN_FIRST]->updateEqualizerGain(ValueConversion::normToValueEqGain(value));
					}
				}
				else if (pid >= PARAM_EQBYPASS_FIRST && pid <= PARAM_EQBYPASS_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_EQBYPASS_FIRST]->switchEqualizerBypass(value);
					}
				}
				else if (pid >= PARAM_ALLPASSDELAY_FIRST && pid <= PARAM_ALLPASSDELAY_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_ALLPASSDELAY_FIRST]->updateAllpassDelay(ValueConversion::normToValueDelay(value));
					}
				}
				else if (pid >= PARAM_ALLPASSDECAY_FIRST && pid <= PARAM_ALLPASSDECAY_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_ALLPASSDECAY_FIRST]->updateAllpassDecay(ValueConversion::normToValueDecay(value));
					}
				}
				else if (pid >= PARAM_ALLPASSBYPASS_FIRST && pid <= PARAM_ALLPASSBYPASS_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_ALLPASSBYPASS_FIRST]->switchAllpassBypass(value);
					}
				}
				else if (pid >= PARAM_OUTGAIN_FIRST && pid <= PARAM_OUTGAIN_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_OUTGAIN_FIRST]->updateOutputGain(ValueConversion::normToValueGain(value));
					}
				}
				else if (pid >= PARAM_OUTBYPASS_FIRST && pid <= PARAM_OUTBYPASS_LAST) {
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						apModules[pid - PARAM_OUTBYPASS_FIRST]->switchOutputBypass(value);
					}
				}
				//...
				else if (pid >= PARAM_GENERALVSTOUTPUTSELECT_FIRST && pid <= PARAM_GENERALVSTOUTPUTSELECT_LAST) {
					// Get only the last change of the value
					if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {
						value = ValueConversion::normToValueMixerInputSelect(value);
						if (value == 0) { // <Not Connected> selected
							connectionMatrix->disconnectVstOutput(pid - PARAM_GENERALVSTOUTPUTSELECT_FIRST);
						}
						else if (value - 1 < MAXMODULENUMBER) { // Another module output as input source selected
							connectionMatrix->setModuleToVstConnection(value - 1, pid - PARAM_GENERALVSTOUTPUTSELECT_FIRST);
						}
						else { // VST input as input source selected
							connectionMatrix->setVstToVstConnection(value - 1 - MAXMODULENUMBER, pid - PARAM_GENERALVSTOUTPUTSELECT_FIRST);
						}
					}
				}
			}
		}
	}


	// Process the audio samples
	if (data.numSamples > 0)
	{
		// Reset PPM values
		std::fill(ppmValues.begin(), ppmValues.end(), 0.0);

		uint32 numberOfSamples = data.numSamples;

		// Get the input buffers for all inputs
		// Sample format is -1.0 to +1.0
		float* inputSamples[MAXVSTINPUTS];
		float* outputSamples[MAXVSTOUTPUTS];

		for (uint32 input = 0; input < MAXVSTINPUTS; ++input) {
			inputSamples[input] = data.inputs[input].channelBuffers32[0];
			
		}
		for (uint32 output = 0; output < MAXVSTOUTPUTS; ++output) {
			outputSamples[output] = data.outputs[output].channelBuffers32[0];
		}

		// Get the connection matrix
		const std::vector<std::vector<short>>& moduleInputConnections = connectionMatrix->getModuleInputConnections();
		const std::vector<short>& vstOutputConnections = connectionMatrix->getVstOutputConnections();

		// Vector with all samples for all inputs which are connected to a module input
		std::vector<double> samplesToProcess;

		// Sample interval
		for (uint32 sample = 0; sample < numberOfSamples; ++sample) {
			// Module input processing
			for (uint16 module = 0; module < MAXMODULENUMBER; ++module) {
				samplesToProcess.clear();
				// For each module input: check if the input is connected to a VST input
				for (uint16 moduleInput = 0; moduleInput < MAXMODULEINPUTS; ++moduleInput) {				
					if (moduleInputConnections[module][moduleInput] != -1) {
						if (moduleInputConnections[module][moduleInput] < MAXMODULENUMBER) {
							// Input is connected to another module's output => take sample from module input buffer
							samplesToProcess.push_back(moduleInputBuffer[moduleInputConnections[module][moduleInput]]);
						}
						else {
							// Input is connected to a VST input => take sample from the VST input
							short mappedVstInput = moduleInputConnections[module][moduleInput];
							samplesToProcess.push_back((double)(inputSamples[connectionMatrix->unmapVstInput(mappedVstInput)][sample]));
						}
					}
					else {
						// Input isn't connected => sample value is zero
						samplesToProcess.push_back(0.0);
					}
				}
				// Process the vector and write the output sample into the correct module output buffer
				moduleOutputBuffer[module] = apModules[module]->processModuleSamples(samplesToProcess);
				// Update PPM values
				if (moduleOutputBuffer[module] > ppmValues[module]) {
					ppmValues[module] = moduleOutputBuffer[module];
				}
			}

			// !!! Swap input and output buffers
			double* temp = moduleInputBuffer;
			moduleInputBuffer = moduleOutputBuffer;
			moduleOutputBuffer = temp;

			// VST output processing
			for (uint16 vstOutput = 0; vstOutput < MAXVSTOUTPUTS; ++vstOutput) {
				if (vstOutputConnections[vstOutput] != -1) {
					if (vstOutputConnections[vstOutput] < MAXMODULENUMBER) {
						// VST output is connected to a module's output => take sample from the module output buffer
						outputSamples[vstOutput][sample] = (moduleOutputBuffer[vstOutputConnections[vstOutput]]);
					}
					else {
						// VST output is connected directly to VST input => take sample from the VST input
						short mappedVstInput = vstOutputConnections[vstOutput];
						outputSamples[vstOutput][sample] = inputSamples[connectionMatrix->unmapVstInput(mappedVstInput)][sample];
					}
				}
				else {
					// Output isn't connected => sample value is zero
					outputSamples[vstOutput][sample] = 0.0;
				}
			}
		}

		//---3) Write outputs parameter changes-----------
		IParameterChanges* paramChanges = data.outputParameterChanges;
		// a new value of VuMeter will be send to the host 
		// (the host will send it back in sync to our controller for updating our editor)

		

		if (paramChanges) {
				for (uint32 i = 0; i < MAXMODULENUMBER; ++i) {
					if (ppmValues[i] != ppmOldValues[i]) {
						int32 index = 0;
						IParamValueQueue* paramQueue = paramChanges->addParameterData(PARAM_PPMUPDATE_FIRST + i, index);
						if (paramQueue) {
							int32 index2 = 0;
							paramQueue->addPoint(0, ppmValues[i], index2);
							ppmOldValues[i] = ppmValues[i];
						}
					}
				}
			}
		

		/*if (paramChanges && fVuPPMOld != fVuPPM)
		{
			int32 index = 0;
			IParamValueQueue* paramQueue = paramChanges->addParameterData(kVuPPMId, index);
			if (paramQueue)
			{
				int32 index2 = 0;
				paramQueue->addPoint(0, fVuPPM, index2);
			}
		}
		fVuPPMOld = fVuPPM;*/


		/*FILE* pFile = fopen("E:\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", "TEST");
		fclose(pFile);*/
	}
	return kResultTrue;
}

}} // namespaces
