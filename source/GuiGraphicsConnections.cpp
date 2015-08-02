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

#include "GuiGraphicsConnections.h"
#include "../vstgui4/vstgui/lib/cdrawcontext.h"
#include "../vstgui4/vstgui/lib/cbitmap.h"

namespace VSTGUI {

	// Some size definitions
	const int inoutRectWidth = 8;
	const int inoutRectHeight = 10;
	const int spacing = 2;

	GuiGraphicsConnections::GuiGraphicsConnections(const CRect& size, const int& maxConnectionNumber) 
		: CViewContainer(size), mouseStart(0), mouseEnd(0) {
		this->setBackgroundColor(CColor(0, 0, 0, 0));
	}

	GuiGraphicsConnections::~GuiGraphicsConnections() {

	}

	void GuiGraphicsConnections::drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect)
	{
		if (getDrawBackground())
		{
			CRect oldClip;
			pContext->getClipRect(oldClip);
			CRect newClip(_updateRect);
			newClip.bound(oldClip);
			pContext->setClipRect(newClip);
			CRect tr(0, 0, getViewSize().getWidth(), getViewSize().getHeight());
			getDrawBackground()->draw(pContext, tr, backgroundOffset);
			pContext->setClipRect(oldClip);
		}
		else if ((backgroundColor.alpha != 255 && getTransparency()) || !getTransparency())
		{
			pContext->setDrawMode(kAliasing);
			pContext->setLineWidth(1);
			pContext->setFillColor(backgroundColor);
			pContext->setFrameColor(backgroundColor);
			pContext->setLineStyle(kLineSolid);
			CRect r;
			if (backgroundColorDrawStyle == kDrawFilled || (backgroundColorDrawStyle == kDrawFilledAndStroked && backgroundColor.alpha == 255))
			{
				r = _updateRect;
				r.inset(-1, -1);
			}
			else
			{
				r = getViewSize();
				r.offset(-r.left, -r.top);
			}
			pContext->drawRect(r, backgroundColorDrawStyle);
		}

		//---
		// Redraw the connection lines
		redraw(pContext);
	}

	void GuiGraphicsConnections::redraw(CDrawContext* pContext) {
		CPoint line[2];
		// Draw all connection lines
		for (auto&& connection : connections) {
			pContext->setLineWidth(1);
			pContext->setFrameColor(CColor(255, 255, 255, (uint8_t)(255 * connection.transparency)));
			line[0] = connection.startPoint;
			line[1] = connection.endPoint;
			pContext->drawLines(line, 2);
		}
		// Draw the line the user is currently dragging
		if (mouseStart != 0) {
			line[0] = mouseStart;
			line[1] = mouseEnd;
			pContext->setFrameColor(CColor(255, 255, 255));
			pContext->drawLines(line, 2);
		}
	}

	void GuiGraphicsConnections::addConnection(const CPoint& startPoint, const CPoint& endPoint, const double& transparency) {
		// Add a connection line
		connections.push_back(ConnectionLine(startPoint, endPoint, transparency, true));
		// And redraw
		this->setDirty();
	}

	void GuiGraphicsConnections::clearConnections() {
		connections.clear();
		setDirty();
	}

	void GuiGraphicsConnections::updateMouseConnectionLine(const CPoint& startPoint, const CPoint& endPoint) {
		this->mouseStart = startPoint;
		this->mouseEnd = endPoint;
		this->setDirty();
	}

	void GuiGraphicsConnections::finishMouseConnectionLine(const double& transparency) {
		// Add the new drawn line
		connections.push_back(ConnectionLine(mouseStart, mouseEnd, transparency, true));
		setDirty();
	}
}