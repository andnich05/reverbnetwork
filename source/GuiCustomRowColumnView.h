#ifndef GUICUSTOMROWCOLUMNVIEW_H
#define GUICUSTOMROWCOLUMNVIEW_H

#include "../vstgui4/vstgui/lib/crowcolumnview.h"

namespace VSTGUI {
	class GuiCustomRowColumnView : public CRowColumnView {
	public:
		GuiCustomRowColumnView(const CRect& size, Style style = kRowStyle, LayoutStyle layoutStyle = kLeftTopEqualy, CCoord spacing = 0., const CRect& margin = CRect(0., 0., 0., 0.));

		// From CViewContainer
		virtual void drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect);	///< draw the background

		// Set the width of the frame to paint around the view (frame is painted inside the view so beware of collisions with content! In such a case: Increase the margin)
		virtual inline void setFrameWidth(const int& width = 0) { frameWidth = width; }
		// Set the rounding of the frame to paint around the view (ToDo...)
		//virtual inline void setFrameRounding(const int& rounding = 0) { frameRounding = rounding; }
		// Set the color of the frame to paint around the view
		virtual inline void setFrameColor(const CColor& color = CColor(0, 0, 0, 0)) { frameColor = color; }

	private:
		int frameWidth;
		//int frameRounding;
		CColor frameColor;
	};

}

#endif GUICUSTOMROWCOLUMNVIEW_H