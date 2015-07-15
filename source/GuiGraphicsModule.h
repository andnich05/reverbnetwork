#ifndef GUIGRAPHICSMODULE_H
#define GUIGRAPHICSMODULE_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"
#include <vector>

namespace VSTGUI {


	class GuiGraphicsModule : public CViewContainer {
	public:
		GuiGraphicsModule(const std::string& title, const int& numberOfInputs, const bool& hasOutput);
		~GuiGraphicsModule();

		void redraw(CDrawContext* pContext);
		inline void setEnabled(const bool& enabled) { this->enabled = enabled; }
		inline void setInputEnabled(const unsigned int& input, const bool& enabled) { if (input < inputsEnabled.size()) { inputsEnabled[input] = enabled; } }
		void setInputNames(const std::vector<std::string>& inputNames);
		inline void setOutputName(const std::string& name) { outputName = name; }
		void updateInput(const int& input, const double& gainValue);
		void clearInputs();
		inline const bool& isEnabled() const { return enabled; }
		inline const double& getGainValue(const int& input) { return inputGainValues[input]; }
		inline const CPoint& getInputRectCenter(const int& input) { return localToFrame(inputRects[input].getCenter()); }
		inline const CPoint& getOutputRectCenter() { return localToFrame(outputRect.getCenter()); }
		inline const int& getNumberOfUsedInputs() { return numberOfUsedInputs; }
		inline const CPoint& getMouseMoveOutputRect() { return mouseMoveOutputRect; }
		inline const CPoint& getMouseUpCoordinates() { return mouseUpCoordinates; }


		// Overrides
		virtual void drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect);
		virtual CMouseEventResult onMouseDown(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		virtual CMouseEventResult onMouseUp(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		virtual CMouseEventResult onMouseMoved(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		//virtual CMouseEventResult onMouseCancel() VSTGUI_OVERRIDE_VMETHOD;
		

	private:
		bool enabled;
		std::string title;
		CRect completeRegion;
		CRect handleRegion;
		CRect mainRegion;
		int numberOfInputs;
		int numberOfUsedInputs;
		std::vector<bool> inputsEnabled;
		std::vector<double> inputGainValues;
		std::vector<std::string> inputNames;
		std::vector<CRect> inputRects;
		bool hasOutput;
		CRect outputRect;
		std::string outputName;

		CPoint mouseDownCoordinates;
		bool mouseDownInHandleRegion;

		void updateShape();

		bool mouseDownInOutputRect;
		CPoint mouseMoveOutputRect;
		CPoint mouseUpCoordinates;
	};

}

#endif // GUIGRAPHICSMODULE_H