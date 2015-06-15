#include "GuiWorkspaceView.h"


namespace VSTGUI {

GuiWorkspaceView::GuiWorkspaceView(const CRect& size, const CRect& containerSize, int32_t style, CCoord scrollbarWidth, CBitmap* pBackground) 
: CScrollView(size,containerSize, style, scrollbarWidth, pBackground) {

}

//-----------------------------------------------------------------------------
/**
* @param view view which z order position should be changed
* @param newIndex index of new z position
* @return true if z order of view changed
*/
bool GuiWorkspaceView::changeViewZOrder(CView* view, int32_t newIndex)
{

	return true;
}

GuiWorkspaceView::~GuiWorkspaceView() {

}

}