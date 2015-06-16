#ifndef GUIBASEAPMODULE_H
#define GUIBASEAPMODULE_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"


namespace VSTGUI {

#ifndef FOREACHSUBVIEW_REVERSE
#define FOREACHSUBVIEW_REVERSE(reverse) ChildViewConstIterator it; ChildViewConstReverseIterator rit; if (reverse) rit = children.rbegin (); else it = children.begin (); while (reverse ? rit != children.rend () : it != children.end ()) { CView* pV; if (reverse) {	pV = (*rit); rit++; } else { pV = (*it); it++; } {
#endif

class GuiBaseAPModule :
	public CViewContainer
{
public:
	// Handle region is the region where the user can drag the module by pressing and holding the mouse button
	GuiBaseAPModule(const CRect& size, const CRect& handleRegion, unsigned int moduleId, CBaseObject* editor);

	virtual CMouseEventResult onMouseDown(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
	virtual CMouseEventResult onMouseMoved(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
	virtual CMouseEventResult onMouseUp(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;

	//virtual void drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect);	///< draw the background

	virtual void setViewSize(const CRect& rect, bool invalid = true) VSTGUI_OVERRIDE_VMETHOD;

	unsigned int getModuleId ();
	void collapseView(const bool& collapse);
	inline bool isCollapsed() { return collapsed; }

	static const char* kModuleWantsFocus;

private:
	double mousePressedX;
	double mousePressedY;
	bool mousePressed;
	CPoint handleSize;
	CPoint viewSize;
	unsigned int moduleId;
	bool collapsed;
	CBaseObject* editor;
};

}

#endif // GUIBASEAPMODULE_H