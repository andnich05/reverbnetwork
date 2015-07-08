#ifndef GUIGRAPHICSMODULE_H
#define GUIGRAPHICSMODULE_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"
#include <vector>

namespace VSTGUI {

	class GuiGraphicsModule : CViewContainer {
	public:
		GuiGraphicsModule(const CPoint& position, const std::string& title, const std::vector<double>& inputGainValues, const std::vector<std::string>& inputNames);
		~GuiGraphicsModule();

		void redraw(CDrawContext* pContext);
		inline void setEnabled(const bool& enabled) { this->enabled = enabled; }
		void updateInputs(const std::vector<double> inputGainValues, const std::vector<std::string>& inputNames);
		void moveTo(const CPoint& position);	

		// Overrides
		//virtual void drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect);
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
		CPoint position;
		int numberOfInputs;
		std::vector<double> inputGainValues;
		std::vector<std::string> inputNames;
		std::vector<CRect> inputRects;
		CRect outputRect;

		CPoint mouseDownCoordinates;
		bool mouseDownInHandleRegion; // Number states the module id; -1 means "false"

	};

}

#endif // GUIGRAPHICSMODULE_H