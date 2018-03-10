/*
* GuiGraphicsModule: Paint a module or Vst input and output
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

#ifndef GUIGRAPHICSMODULE_H
#define GUIGRAPHICSMODULE_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"
#include "../vstgui4/vstgui/lib/controls/ctextedit.h"
#include <vector>

class GuiGraphicsModule : public VSTGUI::CViewContainer, public VSTGUI::IControlListener {
public:
	GuiGraphicsModule(const std::string& title, const int& numberOfInputs, const bool& hasOutput, const bool& hasGainValue);
	virtual ~GuiGraphicsModule();

	// Redraw everything (is called by setDirty())
	void redraw(VSTGUI::CDrawContext* pContext);
	// Enable or disable an input (Disabled input aren't being drawn)
	inline void setInputEnabled(const unsigned int& input, const bool& enabled) { if (input < inputsEnabled.size()) { inputsEnabled[input] = enabled; updateShape(); } }
	// Change the names of the inputs
	void setInputNames(const std::vector<std::string>& inputNames);
	// Set the output name
	inline void setOutputName(const std::string& name) { outputName = name; }
	// Update an input with a new gain value
	void updateInput(const int& input, const double& gainValue);
	// Clear all inputs (The vectors are not being cleared, but gain values are set to 0.0 and inputEnabled values to false)
	void clearInputs();
	// Get a gain value (in case it has been changed through the graphics view
	inline const double& getGainValue(const int& input) { return inputGainValues[input]; }
	// Get the size (incl. position) of an input rectangle (for dragging connections)
	inline VSTGUI::CRect getInputRect(const int& input) { return VSTGUI::CRect(VSTGUI::CPoint(localToFrame(inputRects[input].getTopLeft())), VSTGUI::CPoint(inputRects[input].getWidth(), inputRects[input].getHeight())); }
	// Get the center coordinates of an input
	inline VSTGUI::CPoint getInputRectCenter(const int& input) { return localToFrame(inputRects[input].getCenter()); }
	// Get the center coordinates of the output
	inline VSTGUI::CPoint getOutputRectCenter() { return localToFrame(outputRect.getCenter()); }
	// Get number of enabled inputs
	inline const int& getNumberOfUsedInputs() { return numberOfUsedInputs; }
	// Get the coordinates of the mouse when a user is drawing a new connection
	inline const VSTGUI::CPoint& getMouseMoveOutputRect() { return mouseMoveOutputRect; }
	// Get the coordinates of the mouse when a user finishes a new connection
	inline const VSTGUI::CPoint& getMouseUpCoordinates() { return mouseUpCoordinates; }
	// Get the coordinates of the mouse when a user starts a new connection
	inline const VSTGUI::CPoint& getMouseDownCoordinates() { return mouseDownCoordinates; }
	// Get the input whose gain value has been changed
	inline const int& getInputToUpdate() { return inputToUpdate; }
	inline void setTitle(const std::string& title) { this->title = title; }

	// Overrides from CViewContainer
	virtual void drawBackgroundRect(VSTGUI::CDrawContext* pContext, const VSTGUI::CRect& _updateRect) override;
	virtual VSTGUI::CMouseEventResult onMouseDown(VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons) override;
	virtual VSTGUI::CMouseEventResult onMouseUp(VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons) override;
	virtual VSTGUI::CMouseEventResult onMouseMoved(VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons) override;

	// Override from CControlListener
	virtual void valueChanged(VSTGUI::CControl* pControl);
		

private:
	std::string title; // Title of the module
	VSTGUI::CRect completeRegion; // Complete region of the module (should be the same as the view size)
	VSTGUI::CRect handleRegion; // Handle region for dragging
	VSTGUI::CRect mainRegion; // Main region with the inputs, output and gain values
	int numberOfInputs; // Maximum number of inputs
	int numberOfUsedInputs; // Number of active and visible inputs
	std::vector<bool> inputsEnabled; // Is an input active/visible?
	std::vector<double> inputGainValues; // Gain values for all inputs
	std::vector<VSTGUI::CTextLabel*> inputNames; // Input names
	std::vector<VSTGUI::CRect> inputRects; // Input rectangle regions
	bool hasOutput; // Has the module an output? (a Vst output has no output)
	VSTGUI::CRect outputRect; // Region of the output
	std::string outputName; // Name of the output

	std::vector<VSTGUI::CTextEdit*> textEditsGainValues; // User can change the gain value of an input

	VSTGUI::CPoint mouseDownCoordinates; // Coordinates where the user has pressed down the mouse
	bool mouseDownInHandleRegion; // Has the user pressed the mouse in the handle region?

	void updateShape(); // Update the complete shape (e.g. when the number of active inputs changes)

	bool mouseDownInOutputRect; // Has the user pressed the mouse in the output rect
	VSTGUI::CPoint mouseMoveOutputRect; // Coordinates where the user has moved the mouse after clicking on an output
	VSTGUI::CPoint mouseUpCoordinates;// Coordinates where the user has released the mouse after clicking on an output

	int inputToUpdate; // Contains the input whose gain value the user changed
};

#endif // GUIGRAPHICSMODULE_H