#include "GuiGraphicsConnections.h"
#include "../vstgui4/vstgui/lib/cdrawcontext.h"
#include "../vstgui4/vstgui/lib/cbitmap.h"


namespace VSTGUI {

	const int inoutRectWidth = 8;
	const int inoutRectHeight = 10;
	const int spacing = 2;

	GuiGraphicsConnections::GuiGraphicsConnections(const CRect& size, const int& maxConnectionNumber) 
		: CViewContainer(size), mouseStart(0), mouseEnd(0) {
		/*ConnectionLine connection = ConnectionLine(0, 0, 1.0, false);
		connections.resize(maxConnectionNumber, connection);*/
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

		redraw(pContext);
	}

	void GuiGraphicsConnections::redraw(CDrawContext* pContext) {
		
		CPoint line[2];
		for (auto&& connection : connections) {
			pContext->setLineWidth(1);
			pContext->setFrameColor(CColor(255, 255, 255, (uint8_t)(255 * connection.transparency)));
			line[0] = connection.startPoint;
			line[1] = connection.endPoint;
			pContext->drawLines(line, 2);
			
		}
		if (mouseStart != 0) {
			line[0] = mouseStart;
			line[1] = mouseEnd;
			pContext->setFrameColor(CColor(255, 255, 255));
			pContext->drawLines(line, 2);
		}
	}

	void GuiGraphicsConnections::addConnection(const CPoint& startPoint, const CPoint& endPoint, const double& transparency) {
		//if (id < connections.size()) {
		connections.push_back(ConnectionLine(startPoint, endPoint, transparency, true));
		this->setDirty();
		//}
	}

	void GuiGraphicsConnections::clearConnections() {
		//if (id < connections.size()) {
			/*connections[id].startPoint = 0;
			connections[id].endPoint = 0;
			connections[id].transparency = 1.0;*/
		//}
		connections.clear();
		setDirty();
	}

	void GuiGraphicsConnections::updateMouseConnectionLine(const CPoint& startPoint, const CPoint& endPoint) {
		this->mouseStart = startPoint;
		this->mouseEnd = endPoint;
		this->setDirty();
	}

	void GuiGraphicsConnections::finishMouseConnectionLine(const double& transparency) {
		connections.push_back(ConnectionLine(mouseStart, mouseEnd, transparency, true));
		setDirty();
	}

	//CMouseEventResult GuiGraphicsConnections::onMouseDown(CPoint &where, const CButtonState& buttons)
	//{

	//	/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
	//	fprintf(pFile, "y(n): %s\n", std::to_string(this->getWidth()).c_str());
	//	fclose(pFile);*/

	//	return kMouseEventNotHandled;
	//}

	//CMouseEventResult GuiGraphicsConnections::onMouseMoved(CPoint &where, const CButtonState& buttons)
	//{

	//	return kMouseEventNotHandled;
	//}

	//CMouseEventResult GuiGraphicsConnections::onMouseUp(CPoint& where, const CButtonState& buttons) {

	//	return kMouseEventHandled;
	//}
}