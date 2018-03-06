/*
* GuiSignalGenerator: View for the signal generator
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

#ifndef GUISIGNALGENERATOR_H
#define GUISIGNALGENERATOR_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"

#include "public.sdk/source/vst/vstguieditor.h"

class COptionMenu;
class CTextEdit;
class CTextButton;
class GuiCustomTextLabel;
class GuiCustomValueEdit;

class GuiSignalGenerator : public CViewContainer, VSTGUI::IControlListener {
public:
	GuiSignalGenerator(const CRect& size, Steinberg::Vst::VSTGUIEditor* editor);
	// Update the GUI elements with Controller values
	void updateFromController();
	// Update a parameter by parameter tag
	void updateParameter(const unsigned long int& tag, const double& value);

private:
	// Inititalize
	void createGui();
	// Called when the user changes something
	virtual void valueChanged(VSTGUI::CControl* pControl);

private:
	// Gui elements
	GuiCustomTextLabel* labelTitle;
	GuiCustomTextLabel* labelSignalType;
	VSTGUI::COptionMenu* menuSignalType;
	GuiCustomTextLabel* labelAmplitude;
	GuiCustomValueEdit* editAmplitude;
	GuiCustomTextLabel* labelWidth;
	GuiCustomValueEdit* editWidth;
	VSTGUI::CTextButton* buttonAutoFire;
	GuiCustomTextLabel* labelFireTime;
	GuiCustomValueEdit* editFireTime;
	VSTGUI::CTextButton* buttonManualFire;

	Steinberg::Vst::VSTGUIEditor* editor; // Pointer to ReverbNetworkEditor
};

#endif GUISIGNALGENERATOR_H // GUISIGNALGENERATOR_H