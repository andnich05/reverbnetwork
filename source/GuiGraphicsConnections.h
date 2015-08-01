#ifndef GUIGRAPHICSCONNECTIONS_H
#define GUIGRAPHICSCONNECTIONS_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"
#include <vector>

namespace VSTGUI {
	class GuiGraphicsConnections : public CViewContainer {
	public:
		GuiGraphicsConnections(const CRect& size, const int& maxConnectionNumber);
		~GuiGraphicsConnections();

		// Connection line used for drawing
		struct ConnectionLine {
			ConnectionLine(CPoint startPoint = 0, CPoint endPoint = 0, double transparency = 1.0, bool redraw = true) 
				: startPoint(startPoint), endPoint(endPoint), transparency(transparency) {}
			CPoint startPoint;
			//std::vector<CPoint> interPoints; 
			CPoint endPoint;
			// Direct, rectangle...
			double transparency;
		};

		// Redraw all connection lines
		void redraw(CDrawContext* pContext);
		// Add a connection line
		void addConnection(const CPoint& startPoint, const CPoint& endPoint, const double& transparency);
		// Remove all connection lines
		void clearConnections();
		// Update the line which the user is currently drawing by moving his mouse
		void updateMouseConnectionLine(const CPoint& startPoint, const CPoint& endPoint);
		// Finish the line which the user is currently drawing (when the user releases mouse)
		void finishMouseConnectionLine(const double& transparency);

		// Override from CViewContainer
		virtual void drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect);

	private:
		std::vector<ConnectionLine> connections; // Vector with all connections to draw
		CPoint mouseStart; // Start point of the line the user is currently drawing
		CPoint mouseEnd; // End/current point of the line the user is currently drawing
	}; 

}

#endif // GUIGRAPHICSCONNECTIONS_H