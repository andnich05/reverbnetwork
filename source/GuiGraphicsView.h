#ifndef GUIGRAPHICSVIEW_H
#define GUIGRAPHICSVIEW_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"
#include <vector>

namespace VSTGUI {
	class GuiGraphicsModule;

	class GuiGraphicsView :
		public CViewContainer
	{
	public:
		GuiGraphicsView(const CRect& size, const int& numberOfModules);
		~GuiGraphicsView();

		virtual void addModule(const std::string& title, const int& id, const CPoint& position, const std::vector<double>& inputGainValues, const std::vector<std::string>& inputNames);
		//virtual void setModuleEnabled(const int& id, const bool& enabled);
		virtual void rearrangeModules();

		// Overrides
		virtual void drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect);
		/*virtual CMouseEventResult onMouseDown(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		virtual CMouseEventResult onMouseUp(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		virtual CMouseEventResult onMouseMoved(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;*/
		//virtual CMouseEventResult onMouseCancel() VSTGUI_OVERRIDE_VMETHOD;

	private:
		std::vector<GuiGraphicsModule*>modules;

		virtual void redraw(CDrawContext* pContext);
		virtual void drawModuleRects(CDrawContext* pContext);
		virtual void drawModuleConnections(CDrawContext* pContext);
	};

}

#endif // GUIGRAPHICSVIEW_H