#include "ReverbNetworkEnums.h"

#ifndef REVERBNETWORKDEFINES_H
#define REVERBNETWORKDEFINES_H

// Maximum number of AP modules
#define MAXMODULENUMBER 16

// Maximum number of AP outputs (should be 1 for now)
//#define MAXMODULEOUTPUTS 1


// Maximum number of VST inputs
#define MAXVSTINPUTS 2

// Maximum number of VST outputs
#define MAXVSTOUTPUTS 2

// Limit values to 1.0, prevent clipping
#define LIMITER

// Maximum number of AP inputs
#define MAXMODULEINPUTS 8

// Maximum number of POSSIBLE inputs
#define MAXINPUTS (MAXMODULENUMBER + MAXVSTINPUTS)

//---------------------
// Minimum, Maxmimum, Default values and units

// Mixer input select in menu indexes
#define MIN_MIXERINPUT 0 // <Not Connected>
#define MAX_MIXERINPUT (1 + MAXINPUTS) // Incl. <Not Connected>
#define DEF_MIXERINPUT 0

// Mixer gain given as a factor between -1.0 and +1.0
#define MIN_MIXERGAIN -1.0
#define MAX_MIXERGAIN 1.0
#define DEF_MIXERGAIN 0.0
#define UNIT_MIXERGAIN ""

// Mixer bypass in bool
#define MIN_MIXERBYPASS 0.0
#define MAX_MIXERBYPASS 1.0
#define DEF_MIXERBYPASS 0.0

// Quantizer bitdepth in bits
#define MIN_QUANTIZERBITDEPTH 1.0
#define MAX_QUANTIZERBITDEPTH 32.0
#define DEF_QUANTIZERBITDEPTH 32.0
#define UNIT_QUANTIZERBITDEPTH "bit"

// Quantizer Bypass in bool
#define MIN_QUANTIZERBYPASS 0.0
#define MAX_QUANTIZERBYPASS 1.0
#define DEF_QUANTIZERBYPASS 0.0

// Equalizer filter type in menu indexes
#define MIN_EQFILTERTYPE 0
#define MAX_EQFILTERTYPE (FilterType::numberOfFilterTypes - 1)
#define DEF_EQFILTERTYPE 0

// Equalizer Center frequency in Hertz
#define MIN_EQCENTERFREQ 20.0
#define MAX_EQCENTERFREQ 20000.0
#define DEF_EQCENTERFREQ 4000.0
#define UNIT_EQCENTERFREQ "Hz"

// Equalizer QFactor
#define MIN_EQQFACTOR 0.01
#define MAX_EQQFACTOR 16.0
#define DEF_EQQFACTOR 1.0
#define UNIT_EQQFACTOR ""

// Equalizer Gain in dB
#define MIN_EQGAIN -60.0
#define MAX_EQGAIN 60.0
#define DEF_EQGAIN 0.0
#define UNIT_EQGAIN "dB"

// Equalizer Bypass in bool
#define MIN_EQBYPASS 0.0
#define MAX_EQBYPASS 1.0
#define DEF_EQBYPASS 0.0

// Allpass delay in milliseconds
#define MIN_ALLPASSDELAY 0.0
#define MAX_ALLPASSDELAY 1000.0
#define DEF_ALLPASSDELAY 0.0
#define UNIT_ALLPASSDELAY "ms"

//// Allpass delay in samples
//#define MIN_ALLPASSDELAY 0.0
//#define MAX_ALLPASSDELAY 1000.0
//#define DEF_ALLPASSDELAY 0.0

// Allpass decay in seconds
#define MIN_ALLPASSDECAY 0.0
#define MAX_ALLPASSDECAY 10.0
#define DEF_ALLPASSDECAY 0.0
#define UNIT_ALLPASSDECAY "s"

// Allpass bypass in bool
#define MIN_ALLPASSBYPASS 0.0
#define MAX_ALLPASSBYPASS 1.0
#define DEF_ALLPASSBYPASS 0.0

// Output gain in dB
#define MIN_OUTPUTGAIN -60.0
#define MAX_OUTPUTGAIN 60.0
#define DEF_OUTPUTGAIN 0.0
#define UNIT_OUTPUTGAIN "dB"

// Output bypass in bool
#define MIN_OUTPUTBYPASS 0.0
#define MAX_OUTPUTBYPASS 1.0
#define DEF_OUTPUTBYPASS 0.0

// Visibility of a module in bool
#define MIN_MODULEVISIBLE 0.0
#define MAX_MODULEVISIBLE 1.0
#define DEF_MODULEVISIBLE 0.0

// PPM update
#define MIN_PPMVALUE 0.0
#define MAX_PPMVALUE 1.0
#define DEF_PPMVALUE 0.0

// VST output select in menu indexes
#define MIN_OUTPUTSELECT 0
#define MAX_OUTPUTSELECT (1 + MAXINPUTS) // Incl. <Not Connected>
#define DEF_OUTPUTSELECT 0

//---------------------------
// Mapping for VST parameters


// New parameter? Need to change: Defines (here), Controller (create new VST paramter), Processor (process function), 
// PresetReadWrite (initialization of vector), Editor (GUI), Value Conversion

// Mixer
#define PARAM_MIXERINPUTSELECT_FIRST 0
#define PARAM_MIXERINPUTSELECT_LAST (PARAM_MIXERINPUTSELECT_FIRST + MAXMODULENUMBER * MAXMODULEINPUTS - 1)

#define PARAM_MIXERGAIN_FIRST (PARAM_MIXERINPUTSELECT_LAST + 1)
#define PARAM_MIXERGAIN_LAST (PARAM_MIXERGAIN_FIRST + MAXMODULENUMBER * MAXINPUTS - 1)

#define PARAM_MIXERINPUTMUTED_FIRST (PARAM_MIXERGAIN_LAST + 1)
#define PARAM_MIXERINPUTMUTED_LAST (PARAM_MIXERINPUTMUTED_FIRST + MAXMODULENUMBER * MAXINPUTS - 1)

#define PARAM_MIXERINPUTSOLOED_FIRST (PARAM_MIXERINPUTMUTED_LAST + 1)
#define PARAM_MIXERINPUTSOLOED_LAST (PARAM_MIXERINPUTSOLOED_FIRST + MAXMODULENUMBER * MAXINPUTS - 1)


// Quantizer
#define PARAM_QUANTIZERBITDEPTH_FIRST (PARAM_MIXERINPUTSOLOED_LAST + 1)
#define PARAM_QUANTIZERBITDEPTH_LAST (PARAM_QUANTIZERBITDEPTH_FIRST + MAXMODULENUMBER - 1)

#define PARAM_QUANTIZERBYPASS_FIRST (PARAM_QUANTIZERBITDEPTH_LAST + 1)
#define PARAM_QUANTIZERBYPASS_LAST (PARAM_QUANTIZERBYPASS_FIRST + MAXMODULENUMBER - 1)


// Equalizer
#define PARAM_EQFILTERTYPE_FIRST (PARAM_QUANTIZERBYPASS_LAST + 1)
#define PARAM_EQFILTERTYPE_LAST (PARAM_EQFILTERTYPE_FIRST + MAXMODULENUMBER - 1)

#define PARAM_EQCENTERFREQ_FIRST (PARAM_EQFILTERTYPE_LAST + 1)
#define PARAM_EQCENTERFREQ_LAST (PARAM_EQCENTERFREQ_FIRST + MAXMODULENUMBER - 1)

#define PARAM_EQQFACTOR_FIRST (PARAM_EQCENTERFREQ_LAST + 1)
#define PARAM_EQQFACTOR_LAST (PARAM_EQQFACTOR_FIRST + MAXMODULENUMBER - 1)

#define PARAM_EQGAIN_FIRST (PARAM_EQQFACTOR_LAST + 1)
#define PARAM_EQGAIN_LAST (PARAM_EQGAIN_FIRST + MAXMODULENUMBER - 1)

#define PARAM_EQBYPASS_FIRST (PARAM_EQGAIN_LAST + 1)
#define PARAM_EQBYPASS_LAST (PARAM_EQBYPASS_FIRST + MAXMODULENUMBER - 1)


// Allpass
#define PARAM_ALLPASSDELAY_FIRST (PARAM_EQBYPASS_LAST + 1)
#define PARAM_ALLPASSDELAY_LAST (PARAM_ALLPASSDELAY_FIRST + MAXMODULENUMBER - 1)

#define PARAM_ALLPASSDECAY_FIRST (PARAM_ALLPASSDELAY_LAST + 1)
#define PARAM_ALLPASSDECAY_LAST (PARAM_ALLPASSDECAY_FIRST + MAXMODULENUMBER - 1)

#define PARAM_ALLPASSBYPASS_FIRST (PARAM_ALLPASSDECAY_LAST + 1)
#define PARAM_ALLPASSBYPASS_LAST (PARAM_ALLPASSBYPASS_FIRST + MAXMODULENUMBER - 1)


// Output
#define PARAM_OUTGAIN_FIRST (PARAM_ALLPASSBYPASS_LAST + 1)
#define PARAM_OUTGAIN_LAST (PARAM_OUTGAIN_FIRST + MAXMODULENUMBER - 1)

#define PARAM_OUTBYPASS_FIRST (PARAM_OUTGAIN_LAST + 1)
#define PARAM_OUTBYPASS_LAST (PARAM_OUTBYPASS_FIRST + MAXMODULENUMBER - 1)


// Is a module visible?
#define PARAM_MODULEVISIBLE_FIRST (PARAM_OUTBYPASS_LAST + 1)
#define PARAM_MODULEVISIBLE_LAST (PARAM_MODULEVISIBLE_FIRST + MAXMODULENUMBER - 1)

// General parameters (outside the modules)
#define PARAM_GENERALVSTOUTPUTSELECT_FIRST (PARAM_MODULEVISIBLE_LAST + 1)
#define PARAM_GENERALVSTOUTPUTSELECT_LAST (PARAM_GENERALVSTOUTPUTSELECT_FIRST + MAXVSTOUTPUTS - 1)





// ---------------
// Intern parameters which aren't added to the Controller

// PPM update parameters
#define PARAM_PPMUPDATE_FIRST (PARAM_GENERALVSTOUTPUTSELECT_LAST + 1)
#define PARAM_PPMUPDATE_LAST (PARAM_PPMUPDATE_FIRST + MAXMODULENUMBER - 1)

// ------



// ...



#endif // REVERBNETWORKDEFINES_H