# Reverb Network VST Audio Plug-in
Copyright (c) 2014-2015 by Andrej Nichelmann and Klaus Michael Indlekofer. All rights reserved.
Note: Special restrictions apply. See disclaimers below and within the distribution.

## Summary
Reverb Network is an audio plug-in which is based on Steinberg's VST Technology. Its main purpose is the creation of reverb algorithms by connecting all-pass filter modules to a fully configurable modular network in its build-in user interface. Currently only Windows is supported (since Windows 7).

 ![image](https://github.com/andnich05/reverbnetwork/raw/master/img/ReverbNetworkGuiOverview.png)

This project is currently hosted on:
* GitHub: https://github.com/andnich05/reverbnetwork
* SourceForge: https://sourceforge.net/projects/reverbnetwork/

This software is OPEN SOURCE and is published under the GNU GENERAL PUBLIC LICENSE (see COPYING for details)

This project uses:
* Steinberg VST3 - https://www.steinberg.net/en/company/technologies/vst3.html
* Steinberg VSTGUI - http://sourceforge.net/projects/vstgui/ (possibly outdated)
* pugixml - http://pugixml.org/


## Compiling
To build the plug-in by yourself you will need the following:

* Microsoft Visual Studio 2013 or newer (Express version should be fine, although has not been tested yet)
* Steinberg VST3 3.6.0 or newer SDK (+ VSTGUI 4.2 or newer):https://www.steinberg.net/de/company/developer.html

Keep in mind that Steinberg is constantly changing stuff in the SDK so it is possible that the code will need some changes if a newer SDK version than 3.6.0 is used.

Put the folder "reverbnetwork" into the folder "public.sdk" of the VST3 SDK. Start the "ReverbNetwork.sln" (the structure should be: "public.sdk/reverbnetwork/ReverbNetwork.sln") and build the solution. You can build both the x86 and the x64 version of the plug-in. Make sure you build the plug-in as Release, the Debug build is VERY slow. You can customize the plug-in in the file "ReverbNetworkDefines.h".


## Installation

You will need the appropriate Visual C++ Redistributable Package 2013 (x86 or x64):
https://www.microsoft.com/de-de/download/details.aspx?id=40784

The plug-in's name should be "ReverbNetwork.vst3" (32 bit) or "ReverbNetworkX64.vst3" (64 bit). Just put it in a folder where your VST3 compatible host can find it. If you want to use it in a VST2 host then simply rename the file extension from "vst3" to "dll".


## Tested VST Hosts

Adobe Audition CS6 5.0.2.7:
VST2: incompatible (wrong parameters)
VST3: compatible (no output signal when applying the signal from the effect rack; loading a preset takes much time and blocks the GUI)
	
Steinberg Cubase Elements 8 8.0.10.427
VST2: compatible
VST3: compatible
	
Cockos Reaper 4.77
VST2: compatible (some keyboard entries are not passed to the plug-in e.g. SPACE)

Wavosaur 1.1.0.0
VST2: compatible (Using more than two channels per input and output is not possible; possible crash when closing Wavosaur when the plug-in was compiled on Windows 8/8.1)

Audacity 2.1.1
VST2: compatible (preview function does not output a signal)


## FAQ

Q: My VST host does not support VST3 plug-ins. Can I use the plug-in anyway?

A: Yes, simply rename the file extension from "vst3" to "dll".


## Contact

Andrej Nichelmann (andnich05) - andnich05dev@gmail.com
Klaus Michael Indlekofer (kmi9000) - m.indlekofer@gmx.de

---
The following holds for all files in this distribution (unless stated otherwise on an
individual basis for each file and statement):

These files are distributed in the
hope that they will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. In no event shall the authors be
liable for any direct, indirect, incidental, special, exemplary, or consequential damages
(including, but not limited to, procurement of substitue goods or services; loss of use,
data, or profits; or business interruption) however caused and on any theory of liability,
whether in contract, strict liability, or tort (including negligence or otherwise) arising
in any way out of the use of data/information/software from this distribution, even if
advised of the possibility of such damage. 

The contents of this distribution are intended for educational, non-commercial purposes
only. Materials contained herein are property of their respective owners. If any
copyrighted works/trademarks have been used, please contact the authors A. Nichelmann and K. M. Indlekofer
and the item will be either removed or properly credited (at the copyright/trademark
owner's discretion). We have no intention of violating any copyrights or trademarks.
This distribution might use inlining and deep-linking, i.e. links in this distribution
might lead directly to materials on other web sites/distributions (in which case the
target page normally should be listed/credited in a "links" section). The authors do
not take responsibility for the contents of any links referred to. We do not necessarily 
endorse, sanction, support, encourage, verify or agree with the contents, opinions or
statements of/on any of the linked pages. These statements hold for all links/references
in all files in this distribution. We are in no way affiliated with any
companies/institutions/individuals which might be mentioned in any manner in this
distribution.

The authors do not take responsibility for incorrect, incomplete or misleading information.
Statements are to be considered as the authors' free personal opinion. The authors do not
necessarly possess any of the items mentioned in files in this distribution.

Files (and the information therein) created by the authors A.Nichelmann and K. M. Indlekofer are copyright
(c) by A. Nichelmann and K. M. Indlekofer. Unless protected/restricted otherwise, the authors permit
reproduction/redistribution of material contained in this distribution under the condition
that the item is properly credited. Links to items/materials in this distribution are welcome.
Projects/publications/papers that make use of materials, programs, or generated output
of this distribution must properly credit the authors and mention the usage of this distribution.
Please contact the authors A. Nichelmann and K. M. Indlekofer for comments or further questions
and permission to use materials/information from this distribution.

Some programs/data files are released under the terms of the GNU General Public License
as published by the Free Software Foundation. (Address: Free Software Foundation, Inc.,
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. Internet: GNU General Public
License (GPL) from The GNU Project) 
---
