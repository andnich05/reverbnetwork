/*
* GuiGraphicsConnections: Paint connections between components
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

#ifndef GUIGRAPHICSCONNECTIONS_H
#define GUIGRAPHICSCONNECTIONS_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"
#include <vector>

class GuiGraphicsConnections : public VSTGUI::CViewContainer {
public:
	GuiGraphicsConnections(const VSTGUI::CRect& size, const int& maxConnectionNumber);

	// Connection line used for drawing
	struct ConnectionLine {
		ConnectionLine(VSTGUI::CPoint startPoint = VSTGUI::CPoint(0, 0), VSTGUI::CPoint endPoint = VSTGUI::CPoint(0, 0), double transparency = 1.0, bool redraw = true)
			: startPoint(startPoint), endPoint(endPoint), transparency(transparency) {}
		VSTGUI::CPoint startPoint;
		VSTGUI::CPoint endPoint;
		double transparency;
	};

	// Redraw all connection lines
	void redraw(VSTGUI::CDrawContext* pContext);
	// Add a connection line
	void addConnection(const VSTGUI::CPoint& startPoint, const VSTGUI::CPoint& endPoint, const double& transparency);
	// Remove all connection lines
	void clearConnections();
	// Update the line which the user is currently drawing by moving his mouse
	void updateMouseConnectionLine(const VSTGUI::CPoint& startPoint, const VSTGUI::CPoint& endPoint);
	// Finish the line which the user is currently drawing (when the user releases mouse)
	void finishMouseConnectionLine(const double& transparency);

	// Override from CViewContainer
	virtual void drawBackgroundRect(VSTGUI::CDrawContext* pContext, const VSTGUI::CRect& _updateRect) override;

private:
	std::vector<ConnectionLine> connections; // Vector with all connections to draw
	VSTGUI::CPoint mouseStart; // Start point of the line the user is currently drawing
	VSTGUI::CPoint mouseEnd; // End/current point of the line the user is currently drawing
};

#endif // GUIGRAPHICSCONNECTIONS_H