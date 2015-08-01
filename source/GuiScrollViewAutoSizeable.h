// UNUSED

#ifndef GUISCROLLVIEWAUTOSIZEABLE_H
#define GUISCROLLVIEWAUTOSIZEABLE_H

#include "cscrollview.h"

namespace VSTGUI {

#ifndef FOREACHSUBVIEW_REVERSE
#define FOREACHSUBVIEW_REVERSE(reverse) ChildViewConstIterator it; ChildViewConstReverseIterator rit; if (reverse) rit = children.rbegin (); else it = children.begin (); while (reverse ? rit != children.rend () : it != children.end ()) { CView* pV; if (reverse) {	pV = (*rit); rit++; } else { pV = (*it); it++; } {
#endif

	class GuiScrollViewAutoSizeable :
		public CScrollView
	{
	public:
		GuiScrollViewAutoSizeable(const CRect& size, const CRect& containerSize, int32_t style, CCoord scrollbarWidth = 16, CBitmap* pBackground = 0);
		~GuiScrollViewAutoSizeable();

		CMessageResult notify(CBaseObject* sender, IdStringPtr message) VSTGUI_OVERRIDE_VMETHOD;

		inline void setSubViewDragRegion(const CRect& region) { subViewDragRegion = region; }

		virtual CMouseEventResult onMouseDown(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		virtual CMouseEventResult onMouseUp(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		virtual CMouseEventResult onMouseMoved(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;

	private:
		CRect subViewDragRegion;
	};

	
} 

#endif // GUISCROLLVIEWAUTOSIZEABLE_H