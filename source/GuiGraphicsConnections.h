#ifndef GUIGRAPHICSCONNECTIONS_H
#define GUIGRAPHICSCONNECTIONS_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"
#include <vector>

namespace VSTGUI {

	class GuiGraphicsConnections : public CViewContainer {
	public:
		GuiGraphicsConnections(const CRect& size, const int& maxConnectionNumber);
		~GuiGraphicsConnections();

		struct ConnectionLine {
			ConnectionLine(CPoint startPoint = 0, CPoint endPoint = 0, double transparency = 1.0, bool redraw = true) 
				: startPoint(startPoint), endPoint(endPoint), transparency(transparency) {}

			CPoint startPoint;
			//std::vector<CPoint> interPoints;
			CPoint endPoint;
			// Direct, rectangle...
			double transparency;
		};

		void redraw(CDrawContext* pContext);
		void setConnection(const CPoint& startPoint, const CPoint& endPoint, const double& transparency, const int& id);
		void resetConnection(const int& id);

		// Overrides
		virtual void drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect);
		/*virtual CMouseEventResult onMouseDown(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		virtual CMouseEventResult onMouseUp(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		virtual CMouseEventResult onMouseMoved(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;*/
		//virtual CMouseEventResult onMouseCancel() VSTGUI_OVERRIDE_VMETHOD;

	private:
		std::vector<ConnectionLine> connections;
	};

}

#endif // GUIGRAPHICSCONNECTIONS_H