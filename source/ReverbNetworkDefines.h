#ifndef REVERBNETWORKDEFINES_H
#define REVERBNETWORKDEFINES_H

// Maximum number of AP modules
#define MAXMODULENUMBER 5

// Maximum number of AP inputs
#define MAXMODULEINPUTS 4

// Maximum number of AP outputs (should be 1 for now)
//#define MAXMODULEOUTPUTS 1

// Maximum delay of an Allpass in seconds
#define MAXDELAY 1

// Maximum decay of an Allpass in seconds
#define MAXDECAY 10

// Maximum number of VST inputs
#define MAXVSTINPUTS 2

// Maximum number of VST outputs
#define MAXVSTOUTPUTS 2

// Limit values to 1.0, prevent clipping
#define LIMITER

//---------------------------
// Mapping for VST parameters

// Mixer
#define PARAM_MIXERINPUTSELECT_FIRST 0
#define PARAM_MIXERINPUTSELECT_LAST PARAM_MIXERINPUTSELECT_FIRST + MAXMODULENUMBER * MAXMODULEINPUTS - 1

#define PARAM_MIXERGAIN_FIRST PARAM_MIXERINPUTSELECT_LAST + 1
#define PARAM_MIXERGAIN_LAST PARAM_MIXERGAIN_FIRST + MAXMODULENUMBER * MAXMODULEINPUTS - 1

#define PARAM_MIXERBYPASS_FIRST PARAM_MIXERGAIN_LAST + 1
#define PARAM_MIXERBYPASS_LAST PARAM_MIXERBYPASS_FIRST + MAXMODULENUMBER - 1


// Equalizer
#define PARAM_EQFILTERTYPE_FIRST PARAM_MIXERBYPASS_LAST + 1
#define PARAM_EQFILTERTYPE_LAST PARAM_EQFILTERTYPE_FIRST + MAXMODULENUMBER - 1

#define PARAM_EQCENTERFREQ_FIRST PARAM_EQFILTERTYPE_LAST + 1
#define PARAM_EQCENTERFREQ_LAST PARAM_EQCENTERFREQ_FIRST + MAXMODULENUMBER - 1

#define PARAM_EQQFACTOR_FIRST PARAM_EQCENTERFREQ_LAST + 1
#define PARAM_EQQFACTOR_LAST PARAM_EQQFACTOR_FIRST + MAXMODULENUMBER - 1

#define PARAM_EQGAIN_FIRST PARAM_EQQFACTOR_LAST + 1
#define PARAM_EQGAIN_LAST PARAM_EQGAIN_FIRST + MAXMODULENUMBER - 1

#define PARAM_EQBYPASS_FIRST PARAM_EQGAIN_LAST + 1
#define PARAM_EQBYPASS_LAST PARAM_EQBYPASS_FIRST + MAXMODULENUMBER - 1


// Allpass
#define PARAM_ALLPASSDELAY_FIRST PARAM_EQBYPASS_LAST + 1
#define PARAM_ALLPASSDELAY_LAST PARAM_ALLPASSDELAY_FIRST + MAXMODULENUMBER - 1

#define PARAM_ALLPASSDECAY_FIRST PARAM_ALLPASSDELAY_LAST + 1
#define PARAM_ALLPASSDECAY_LAST PARAM_ALLPASSDECAY_FIRST + MAXMODULENUMBER - 1

#define PARAM_ALLPASSBYPASS_FIRST PARAM_ALLPASSDECAY_LAST + 1
#define PARAM_ALLPASSBYPASS_LAST PARAM_ALLPASSBYPASS_FIRST + MAXMODULENUMBER - 1


// Output
#define PARAM_OUTGAIN_FIRST PARAM_ALLPASSBYPASS_LAST + 1
#define PARAM_OUTGAIN_LAST PARAM_OUTGAIN_FIRST + MAXMODULENUMBER - 1

#define PARAM_OUTBYPASS_FIRST PARAM_OUTGAIN_LAST + 1
#define PARAM_OUTBYPASS_LAST PARAM_OUTBYPASS_FIRST + MAXMODULENUMBER - 1

#endif // REVERBNETWORKDEFINES_H