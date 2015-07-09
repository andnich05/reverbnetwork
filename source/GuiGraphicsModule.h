#ifndef GUIGRAPHICSMODULE_H
#define GUIGRAPHICSMODULE_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"
#include <vector>

namespace VSTGUI {

	class GuiGraphicsModule : public CViewContainer {
	public:
		GuiGraphicsModule(const std::string& title, const std::vector<std::string>& inputNames);
		~GuiGraphicsModule();

		void redraw(CDrawContext* pContext);
		inline void setEnabled(const bool& enabled) { this->enabled = enabled; }
		void updateInput(const int& input, const double& gainValue);
		inline const bool& isEnabled() const { return enabled; }
		inline const double& getGainValue(const int& input) { return inputGainValues[input]; }
		inline const CPoint& getInputRectCenter(const int& input) { return localToFrame(inputRects[input].getCenter()); }
		inline const CPoint& getOutputRectCenter() { return localToFrame(outputRect.getCenter()); }
		inline const int& getNumberOfUsedInputs() { return numberOfUsedInputs; }

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
		std::vector<double> inputGainValues;
		std::vector<std::string> inputNames;
		std::vector<CRect> inputRects;
		CRect outputRect;

		CPoint mouseDownCoordinates;
		bool mouseDownInHandleRegion;

		void updateShape();
	};

}

#endif // GUIGRAPHICSMODULE_H