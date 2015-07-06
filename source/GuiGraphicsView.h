#ifndef GUIGRAPHICSVIEW_H
#define GUIGRAPHICSVIEW_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"
#include <vector>

namespace VSTGUI {

	class GuiGraphicsView :
		public CViewContainer
	{
	public:
		GuiGraphicsView(const CRect& size);
		~GuiGraphicsView();

		

		virtual void addModule(const std::string& title, const int& id, const CPoint& position, const std::vector<double>& inputGainValues);
		virtual void removeModule(const int& id);
		virtual void rearrangeModules();

		// Overrides
		virtual void drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect);
		virtual CMouseEventResult onMouseDown(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		virtual CMouseEventResult onMouseUp(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		virtual CMouseEventResult onMouseMoved(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		//virtual CMouseEventResult onMouseCancel() VSTGUI_OVERRIDE_VMETHOD;

	private:
		virtual void redraw(CDrawContext* pContext);
		virtual void drawModuleRects(CDrawContext* pContext);
		virtual void drawModuleConnections(CDrawContext* pContext);
		

		typedef struct Module {
			bool enabled;
			std::string title;
			CRect completeRegion;
			CRect handleRegion;
			CPoint position;
			std::vector<double> inputGainValues;
			int numberOfUsedInputs;
			std::vector<CRect> inputRects;
			CRect outputRect;
		};
		std::vector<Module> modules;

		CPoint mouseDownCoordinates;
		int mouseDownInHandleRegion; // Number states the module id; -1 means "false"
	};

}

#endif // GUIGRAPHICSVIEW_H