
-- SUMMARY --

Reverb Network VST Audio Plug-in
Copyright (c) 2015 by Andrej Nichelmann and Klaus Michael Indlekofer. All rights reserved.

Reverb Network is an audio plug-in which is based on Steinberg's VST Technology. Its main purpose is the creation of reverb algorithms by connecting all-pass filter modules to a network in its build-in user interface. Currently only Windows is supported (since Windows 7).

This project was created as part of the master thesis at the RheinMain University of Applied Sciences and is currently hosted on GitHub:
https://github.com/andnich05/reverbnetwork

This software is OPEN SOURCE and is published under the GNU GENERAL PUBLIC LICENSE (see COPYING for details)

This project uses:
Steinberg VST3 - https://www.steinberg.net/en/company/technologies/vst3.html
Steinberg VSTGUI - http://sourceforge.net/projects/vstgui/ (possibly outdated)
pugixml - http://pugixml.org/


-- COMPILING --

To build the plug-in by yourself you will need the following:

- Microsoft Visual Studio 2013 or newer (Express version should be fine, although has not been tested yet)
- Steinberg VST3 SDK: https://www.steinberg.net/de/company/developer.html

Keep in mind that Steinberg is constantly changing stuff in the SDK so it is possible that the code will need some changes if a newer SDK version than 3.6.0 is used. VSTGUI (4.2 or newer) should come along with the SDK.

Put the folder "reverbnetwork" into the folder "public.sdk" of the VST3 SDK. Start the "ReverbNetwork.sln" (the structure should be: "public.sdk/reverbnetwork/ReverbNetwork.sln") and build the solution. You can build both the x86 and the x64 version of the plug-in. Make sure you build the plug-in as Release, the Debug build is VERY slow. You can customize the plug-in in the file "ReverbNetworkDefines.h".


-- INSTALLATION --

You will need the appropriate Visual C++ Redistributable Package 2013 (x86 or x64):
https://www.microsoft.com/de-de/download/details.aspx?id=40784

The plug-in's name should be "ReverbNetwork.vst3" (32 bit) or "ReverbNetworkX64.vst3" (64 bit). Just put it in a folder where your VST3 compatible host can find it. If you want to use it in a VST2 host then simply rename the file extension from "vst3" to "dll".


-- TESTED VST HOSTS --

Adobe Audition CS6 5.0.2.7:
VST2: incompatible (wrong parameters)
VST3: compatible (no output signal when applying the signal from the effect rack; loading a preset takes much time and blocks the GUI)
	
Steinberg Cubase Elements 8 8.0.10.427
VST2: compatible
VST3: compatible
	
Cockos Reaper 4.77
VST2: compatible (some keyboard entries are not passed to the plug-in e.g. SPACE)

Wavosaur 1.1.0.0
VST2: compatible (Using more than two channels per input and output is not possible; possible crash when the plug-in was compiled on Windows 8/8.1 when closing Wavosaur)

Audacity 2.1.1
VST2: compatible (preview function does not output a signal)


-- FAQ --

Q: My VST host does not support VST3 plug-ins. Can I use the plug-in anyway?

A: Yes, simply rename the file extension from "vst3" to "dll".


-- CONTACT --

Andrej Nichelmann (andnich05) - andnich05dev@gmail.com
Klaus Michael Indlekofer

	