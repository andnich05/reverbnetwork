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

#ifndef REVERBNETWORKPROCESSOR_H
#define REVERBNETWORKPROCESSOR_H

#include "public.sdk/source/vst/vstaudioeffect.h"
#include <vector>
#include <memory>

class BaseAPModule;
class ConnectionMatrix;

namespace Steinberg {
namespace Vst {

//-----------------------------------------------------------------------------
class ReverbNetworkProcessor : public AudioEffect
{
public:
	ReverbNetworkProcessor();
	~ReverbNetworkProcessor();
	
	tresult PLUGIN_API initialize (FUnknown* context);
	tresult PLUGIN_API setBusArrangements (SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts);

	tresult PLUGIN_API setActive (TBool state);
	tresult PLUGIN_API process (ProcessData& data);
	
	static FUnknown* createInstance(void*) { return (IAudioProcessor*)new ReverbNetworkProcessor(); }

protected:
	ParamValue delay;
	float** buffer;
	//int32 bufferPos;

private:
	//std::vector<std::shared_ptr<SchroederAllpass>> allpasses;

	double* moduleInputBuffer;
	double* moduleOutputBuffer;
	std::vector<std::shared_ptr<BaseAPModule>> apModules;

	ConnectionMatrix* connectionMatrix;

	

};

}} // namespaces

#endif // REVERBNETWORKPROCESSOR_H
