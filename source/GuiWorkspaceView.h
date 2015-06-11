#ifndef GUIWORKSPACEVIEW_H
#define GUIWORKSPACEVIEW_H

#include "cscrollview.h"

namespace VSTGUI {

class GuiWorkspaceView :
	public CScrollView
{
public:
	GuiWorkspaceView(const CRect& size, const CRect& containerSize, int32_t style, CCoord scrollbarWidth = 16, CBitmap* pBackground = 0);
	~GuiWorkspaceView();

	bool changeViewZOrder(CView* view, int32_t newIndex);	///< change view z order position
};

} 

#endif // GUIWORKSPACEVIEW_H