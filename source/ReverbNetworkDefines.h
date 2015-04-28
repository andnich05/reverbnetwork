#ifndef REVERBNETWORKDEFINES_H
#define REVERBNETWORKDEFINES_H

// Maximum number of AP modules
#define MAXMODULENUMBER 4

// Maximum number of AP inputs
#define MAXMODULEINPUTS 3

// Maximum number of AP outputs (should be 1 for now)
//#define MAXMODULEOUTPUTS 1

// Maximum delay of an Allpass in seconds
#define MAXDELAY 1

// Maximum decay of an Allpass in seconds
#define MAXDECAY 10

// Maximum number of VST inputs
#define MAXVSTINPUTS 6

// Maximum number of VST outputs
#define MAXVSTOUTPUTS 4

// Limit values to 1.0, prevent clipping
#define LIMITER

//---------------------------
// Mapping for VST parameters
// The order of the parameters MUST NOT BE CHANGED! Additional parameters have to be added at the end!

// First pid for module input gain per channel parameter
#define PARAM_MODULEINGAIN_FIRST 0
// Last pid for channel gain per module parameter
#define PARAM_MODULEINGAIN_LAST MAXMODULENUMBER*MAXMODULEINPUTS-1

// First pid for allpass delay 
#define PARAM_ALLPASSDELAY_FIRST PARAM_MODULEINGAIN_LAST+1
// Last pid for allpass delay
#define PARAM_ALLPASSDELAY_LAST PARAM_MODULEINGAIN_LAST+1+MAXMODULENUMBER-1

// First pid for allpass decay
#define PARAM_ALLPASSDECAY_FIRST PARAM_ALLPASSDELAY_LAST+1
// Last pid for allpass decay
#define PARAM_ALLPASSDECAY_LAST PARAM_ALLPASSDELAY_LAST+1+MAXMODULENUMBER-1

// First pid for module ouput gain
#define PARAM_MODULEOUTGAIN_FIRST PARAM_ALLPASSDECAY_LAST+1
// Last pid for module output gain
#define PARAM_MODULEOUTGAIN_LAST PARAM_ALLPASSDECAY_LAST+1+MAXMODULENUMBER-1

#endif // REVERBNETWORKDEFINES_H