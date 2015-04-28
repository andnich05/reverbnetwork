#ifndef GUIBASEMODULEFRAME_H
#define GUIBASEMODULEFRAME_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"


namespace VSTGUI {

#ifndef FOREACHSUBVIEW_REVERSE
//#define FOREACHSUBVIEW_REVERSE(reverse) ChildViewConstIterator it; ChildViewConstReverseIterator rit; if (reverse) rit = children.rbegin (); else it = children.begin (); while (reverse ? rit != children.rend () : it != children.end ()) { CView* pV; if (reverse) {	pV = (*rit); rit++; } else { pV = (*it); it++; } {
#endif

class GuiBaseModuleFrame :
	public CViewContainer
{
public:
	GuiBaseModuleFrame(const CRect& size);

	virtual CMouseEventResult onMouseDown(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
	virtual CMouseEventResult onMouseMoved(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
	virtual CMouseEventResult onMouseUp(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;

private:
	int mousePressedX;
	int mousePressedY;
	bool mousePressed;
};

}

#endif // GUIBASEMODULEFRAME_H