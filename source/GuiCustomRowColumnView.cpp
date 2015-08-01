#include "GuiCustomRowColumnView.h"
#include "cbitmap.h"

namespace VSTGUI {

	GuiCustomRowColumnView::GuiCustomRowColumnView(const CRect& size, Style style, LayoutStyle layoutStyle, CCoord spacing, const CRect& margin) 
		: CRowColumnView(size, style, layoutStyle, spacing, margin), frameWidth(0), frameColor(CColor(0, 0, 0, 0)) {

	}

	void GuiCustomRowColumnView::drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect) {
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

		// Custom stuff
		pContext->setFillColor(CColor(0, 0, 0, 0));
		pContext->setLineWidth(frameWidth);
		pContext->setFrameColor(frameColor);
		CRect rect = getViewSize();
		rect.offset(-rect.left, -rect.top);
		pContext->drawRect(rect);
	}
}