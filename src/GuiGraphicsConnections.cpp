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

#include "../include/GuiGraphicsConnections.h"

#include "../vstgui4/vstgui/lib/cdrawcontext.h"
#include "../vstgui4/vstgui/lib/cbitmap.h"

// Some size definitions
const int inoutRectWidth = 8;
const int inoutRectHeight = 10;
const int spacing = 2;

GuiGraphicsConnections::GuiGraphicsConnections(const VSTGUI::CRect& size, const int& maxConnectionNumber) 
	: CViewContainer(size)
	, mouseStart(VSTGUI::CPoint(0, 0))
	, mouseEnd(VSTGUI::CPoint(0, 0)) {

	this->setBackgroundColor(VSTGUI::CColor(0, 0, 0, 0));
}

void GuiGraphicsConnections::drawBackgroundRect(VSTGUI::CDrawContext* pContext, const VSTGUI::CRect& _updateRect)
{
	CViewContainer::drawBackgroundRect(pContext, _updateRect);

	// Redraw the connection lines
	redraw(pContext);
}

void GuiGraphicsConnections::redraw(VSTGUI::CDrawContext* pContext) {
	VSTGUI::CDrawContext::LinePair line;
	// Draw all connection lines
	for (auto&& connection : connections) {
		pContext->setLineWidth(1);
		pContext->setFrameColor(VSTGUI::CColor(255, 255, 255, (static_cast< uint8_t >(255 * connection.transparency))));
		line.first = connection.startPoint;
		line.second = connection.endPoint;
		pContext->drawLine(line);
	}
	// Draw the line the user is currently dragging
	if (mouseStart != VSTGUI::CPoint(0, 0)) {
		line.first = mouseStart;
		line.second = mouseEnd;
		pContext->setFrameColor(VSTGUI::CColor(255, 255, 255));
		pContext->drawLine(line);
	}
}

void GuiGraphicsConnections::addConnection(const VSTGUI::CPoint& startPoint, const VSTGUI::CPoint& endPoint, const double& transparency) {
	// Add a connection line
	connections.push_back(ConnectionLine(startPoint, endPoint, transparency, true));
	// And redraw
	this->setDirty();
}

void GuiGraphicsConnections::clearConnections() {
	connections.clear();
	setDirty();
}

void GuiGraphicsConnections::updateMouseConnectionLine(const VSTGUI::CPoint& startPoint, const VSTGUI::CPoint& endPoint) {
	this->mouseStart = startPoint;
	this->mouseEnd = endPoint;
	this->setDirty();
}

void GuiGraphicsConnections::finishMouseConnectionLine(const double& transparency) {
	// Add the new drawn line
	connections.push_back(ConnectionLine(mouseStart, mouseEnd, transparency, true));
	setDirty();
}
