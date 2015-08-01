#ifndef GUIGRAPHICSMODULE_H
#define GUIGRAPHICSMODULE_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"
#include "../vstgui4/vstgui/lib/controls/ctextedit.h"
#include <vector>

namespace VSTGUI {
	class GuiGraphicsModule : public CViewContainer, public CControlListener {
	public:
		GuiGraphicsModule(const std::string& title, const int& numberOfInputs, const bool& hasOutput, const bool& hasGainValue);
		~GuiGraphicsModule();

		// Redraw everything (is called by setDirty())
		void redraw(CDrawContext* pContext);
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
		inline const CRect getInputRect(const int& input) { return CRect(CPoint(localToFrame(inputRects[input].getTopLeft())), CPoint((inputRects[input].getWidth(), inputRects[input].getHeight()))); }
		// Get the center coordinates of an input
		inline const CPoint& getInputRectCenter(const int& input) { return localToFrame(inputRects[input].getCenter()); }
		// Get the center coordinates of the output
		inline const CPoint& getOutputRectCenter() { return localToFrame(outputRect.getCenter()); }
		// Get number of enabled inputs
		inline const int& getNumberOfUsedInputs() { return numberOfUsedInputs; }
		// Get the coordinates of the mouse when a user is drawing a new connection
		inline const CPoint& getMouseMoveOutputRect() { return mouseMoveOutputRect; }
		// Get the coordinates of the mouse when a user finishes a new connection
		inline const CPoint& getMouseUpCoordinates() { return mouseUpCoordinates; }
		// Get the coordinates of the mouse when a user starts a new connection
		inline const CPoint& getMouseDownCoordinates() { return mouseDownCoordinates; }
		// Get the input whose gain value has been changed
		inline const int& getInputToUpdate() { return inputToUpdate; }
		inline void setTitle(const std::string& title) { this->title = title; }

		// Overrides from CViewContainer
		virtual void drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect);
		virtual CMouseEventResult onMouseDown(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		virtual CMouseEventResult onMouseUp(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		virtual CMouseEventResult onMouseMoved(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;

		// Override from CControlListener
		virtual void valueChanged(VSTGUI::CControl* pControl);
		

	private:
		std::string title; // Title of the module
		CRect completeRegion; // Complete region of the module (should be the same as the view size)
		CRect handleRegion; // Handle region for dragging
		CRect mainRegion; // Main region with the inputs, output and gain values
		int numberOfInputs; // Maximum number of inputs
		int numberOfUsedInputs; // Number of active and visible inputs
		std::vector<bool> inputsEnabled; // Is an input active/visible?
		std::vector<double> inputGainValues; // Gain values for all inputs
		std::vector<CTextLabel*> inputNames; // Input names
		std::vector<CRect> inputRects; // Input rectangle regions
		bool hasOutput; // Has the module an output? (a Vst output has no output)
		CRect outputRect; // Region of the output
		std::string outputName; // Name of the output

		std::vector<CTextEdit*> textEditsGainValues; // User can change the gain value of an input

		CPoint mouseDownCoordinates; // Coordinates where the user has pressed down the mouse
		bool mouseDownInHandleRegion; // Has the user pressed the mouse in the handle region?

		void updateShape(); // Update the complete shape (e.g. when the number of active inputs changes)

		bool mouseDownInOutputRect; // Has the user pressed the mouse in the output rect
		CPoint mouseMoveOutputRect; // Coordinates where the user has moved the mouse after clicking on an output
		CPoint mouseUpCoordinates;// Coordinates where the user has released the mouse after clicking on an output

		int inputToUpdate; // Contains the input whose gain value the user changed
	};
}

#endif // GUIGRAPHICSMODULE_H