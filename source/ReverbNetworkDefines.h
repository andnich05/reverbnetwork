#include "ReverbNetworkEnums.h"

// Log file output
//#define LOGGING

#ifndef REVERBNETWORKDEFINES_H
#define REVERBNETWORKDEFINES_H

// Maximum number of AP modules
#define MAXMODULENUMBER 32

// Maximum number of AP outputs (should be 1)
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

// Mixer input select (menu index)
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
#define MIN_QUANTIZERBITDEPTH 1
#define MAX_QUANTIZERBITDEPTH 32
#define DEF_QUANTIZERBITDEPTH 32
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
// Only for the VST Parameter, maximum frequency depends on the Host's sample rate (should be smaller than sampleRate/2)
#define MAX_EQCENTERFREQ 88000.0
#define DEF_EQCENTERFREQ 2000.0
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

// Equalizer Coefficients
#define MIN_EQCOEFFICIENTS -100.0
#define MAX_EQCOEFFICIENTS 100.0
#define DEF_EQCOEFFICIENTS 0.0
#define UNIT_EQCOEFFICIENTS ""

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

// Allpass decay in milliseconds
#define MIN_ALLPASSDECAY 0.0
#define MAX_ALLPASSDECAY 10000.0
#define DEF_ALLPASSDECAY 0.0
#define UNIT_ALLPASSDECAY "ms"

// Allpass diffK sign
#define MIN_ALLPASSDIFFKSIGN 0.0
#define MAX_ALLPASSDIFFKSIGN 1.0
#define DEF_ALLPASSDIFFKSIGN 1.0
#define UNIT_ALLPASSDIFFKSIGN ""

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
// PresetReadWrite (initialization of vector), Editor (valueChanged), Value Conversion, BaseAPModule, XML Preset

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

#define PARAM_EQCOEFFICIENTA0_FIRST (PARAM_EQGAIN_LAST + 1)
#define PARAM_EQCOEFFICIENTA0_LAST (PARAM_EQCOEFFICIENTA0_FIRST + MAXMODULENUMBER - 1)

#define PARAM_EQCOEFFICIENTA1_FIRST (PARAM_EQCOEFFICIENTA0_LAST + 1)
#define PARAM_EQCOEFFICIENTA1_LAST (PARAM_EQCOEFFICIENTA1_FIRST + MAXMODULENUMBER - 1)

#define PARAM_EQCOEFFICIENTA2_FIRST (PARAM_EQCOEFFICIENTA1_LAST + 1)
#define PARAM_EQCOEFFICIENTA2_LAST (PARAM_EQCOEFFICIENTA2_FIRST + MAXMODULENUMBER - 1)

#define PARAM_EQCOEFFICIENTB1_FIRST (PARAM_EQCOEFFICIENTA2_LAST + 1)
#define PARAM_EQCOEFFICIENTB1_LAST (PARAM_EQCOEFFICIENTB1_FIRST + MAXMODULENUMBER - 1)

#define PARAM_EQCOEFFICIENTB2_FIRST (PARAM_EQCOEFFICIENTB1_LAST + 1)
#define PARAM_EQCOEFFICIENTB2_LAST (PARAM_EQCOEFFICIENTB2_FIRST + MAXMODULENUMBER - 1)

#define PARAM_EQBYPASS_FIRST (PARAM_EQCOEFFICIENTB2_LAST + 1)
#define PARAM_EQBYPASS_LAST (PARAM_EQBYPASS_FIRST + MAXMODULENUMBER - 1)


// Allpass
#define PARAM_ALLPASSDELAY_FIRST (PARAM_EQBYPASS_LAST + 1)
#define PARAM_ALLPASSDELAY_LAST (PARAM_ALLPASSDELAY_FIRST + MAXMODULENUMBER - 1)

#define PARAM_ALLPASSDECAY_FIRST (PARAM_ALLPASSDELAY_LAST + 1)
#define PARAM_ALLPASSDECAY_LAST (PARAM_ALLPASSDECAY_FIRST + MAXMODULENUMBER - 1)

#define PARAM_ALLPASSDIFFKSIGN_FIRST (PARAM_ALLPASSDECAY_LAST + 1)
#define PARAM_ALLPASSDIFFKSIGN_LAST (PARAM_ALLPASSDIFFKSIGN_FIRST + MAXMODULENUMBER - 1)

#define PARAM_ALLPASSBYPASS_FIRST (PARAM_ALLPASSDIFFKSIGN_LAST + 1)
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
// Intern parameters which aren't added to the Controller (must be at the end of this list!)

// PPM update parameters
#define PARAM_PPMUPDATE_FIRST (PARAM_GENERALVSTOUTPUTSELECT_LAST + 1)
#define PARAM_PPMUPDATE_LAST (PARAM_PPMUPDATE_FIRST + MAXMODULENUMBER - 1)

// PPM update parameters
#define PARAM_EQSTABILITY_FIRST (PARAM_PPMUPDATE_LAST + 1)
#define PARAM_EQSTABILITY_LAST (PARAM_EQSTABILITY_FIRST + MAXMODULENUMBER - 1)

// Since the editor object is destroyed every time the user closes the gui these parameters need to be saved somewhere => in the Controller
// Module names
#define PARAM_MODULENAME_FIRST (PARAM_EQSTABILITY_LAST + 1)
#define PARAM_MODULENAME_LAST (PARAM_MODULENAME_FIRST + MAXMODULENUMBER - 1)
// Preset name
#define PARAM_PRESETNAME PARAM_MODULENAME_LAST + 1


// ------



// ...


// Color scheme
#define CCOLOR_SPLITVIEW_SPLITTER CColor(0, 0, 0)
#define CCOLOR_WORKSPACE_BACKGROUND CColor(100, 100, 100)
#define CCOLOR_MODULELIST_BACKGROUND CColor(80, 80, 80)
#define CCOLOR_MODULE_MAINBACKGROUND CColor(70, 70, 70)
#define CCOLOR_MODULE_MAINFRAME CColor(20, 20, 20)
#define CCOLOR_MODULE_HANDLEBACKGROUND CColor(70, 70, 70)
#define CCOLOR_MODULE_HANDLEFRAME CColor(20, 20, 20)
#define CCOLOR_MODULE_COMPONENTBACKGROUND CColor(70, 70, 70)
#define CCOLOR_MODULE_COMPONENTFRAME CColor(20, 20, 20)
#define CCOLOR_MODULE_TEXT CColor(255, 255, 255)
// PPM is a .png
//#define CCOLOR_MODULE_PPMOFF CColor(20, 20, 20)
//#define CCOLOR_MODULE_PPMON CColor(229, 83, 0)

#define CCOLOR_GRAPHICSVIEW_BACKGROUND CColor(100, 100, 100)
#define CCOLOR_GRAPHICSVIEW_TOOLBOXBACKGROUND CColor(80, 80, 80)
#define CCOLOR_GRAPHICSVIEW_MODULEBACKGROUND CColor(70, 70, 70)
#define CCOLOR_GRAPHICSVIEW_MODULEHANDLE CColor(70, 70, 70)
#define CCOLOR_GRAPHICSVIEW_VSTINPUTBACKGROUND CColor(150, 150, 150)
#define CCOLOR_GRAPHICSVIEW_VSTINPUTHANDLE CColor(120, 120, 120)
#define CCOLOR_GRAPHICSVIEW_VSTOUTPUTBACKGROUND CColor(150, 150, 150)
#define CCOLOR_GRAPHICSVIEW_VSTOUTPUTHANDLE CColor(120, 120, 120)
#define CCOLOR_GRAPHICSVIEW_CONNECTION CColor(255, 255, 255)

#define CCOLOR_SIDEBAR_BACKGROUND CColor(150, 150, 150)

#define CCOLOR_BUTTON_STARTNORMALBACKGROUND CColor(150, 150, 150)
#define CCOLOR_BUTTON_ENDNORMALBACKGROUND CColor(120, 120, 120)
#define CCOLOR_BUTTON_STARTPRESSEDBACKGROUND CColor(200, 200, 200)
#define CCOLOR_BUTTON_ENDPRESSEDBACKGROUND CColor(180, 180, 180)
#define CCOLOR_BUTTON_TEXTNORMAL CColor(255, 255, 255)
#define CCOLOR_BUTTON_TEXTPRESSED CColor(0, 0, 0)
#define CCOLOR_SCROLLVIEW_SCROLLBAR CColor(140, 140, 140)
#define CCOLOR_OPTIONMENU_BACKGROUND CColor(30, 30, 30)
#define CCOLOR_OPTIONMENU_FRAME CColor(0, 0, 0)
#define CCOLOR_OPTIONMENU_TEXT CColor(255, 255, 255)
#define CCOLOR_TEXTEDIT_BACKGROUND CColor(30, 30, 30)
#define CCOLOR_TEXTEDIT_FRAME CColor(0, 0, 0)
#define CCOLOR_TEXTEDIT_TEXT CColor(255, 255, 255)
#define CCOLOR_TEXTLABEL_BACKGROUND CColor(0, 0, 0, 0)
#define CCOLOR_TEXTLABEL_FRAME CColor(0, 0, 0, 0)
#define CCOLOR_NOCOLOR CColor(0, 0, 0, 0)
#define CCOLOR_FRAME CColor(20, 20, 20)

#define CCOLOR_SPLASHVIEW_BACKGROUND CColor(0, 0, 0, 150)



#endif // REVERBNETWORKDEFINES_H