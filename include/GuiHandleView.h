#ifndef GUIHANDLEVIEW_H
#define GUIHANDLEVIEW_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"


namespace VSTGUI {

#ifndef FOREACHSUBVIEW_REVERSE
#define FOREACHSUBVIEW_REVERSE(reverse) ChildViewConstIterator it; ChildViewConstReverseIterator rit; if (reverse) rit = children.rbegin (); else it = children.begin (); while (reverse ? rit != children.rend () : it != children.end ()) { CView* pV; if (reverse) {	pV = (*rit); rit++; } else { pV = (*it); it++; } {
#endif

	class GuiHandleView :
		public CViewContainer
	{
	public:
		// Handle region is the region where the user can drag the module by pressing and holding the mouse button
		GuiHandleView(const CRect& size);

		virtual CMouseEventResult onMouseDown(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		virtual CMouseEventResult onMouseMoved(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		virtual CMouseEventResult onMouseUp(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;

	private:
		int mousePressedX;
		int mousePressedY;
		bool mousePressed;
		CRect handleRegion;
	};

}

#endif // GUIHANDLEVIEW_H