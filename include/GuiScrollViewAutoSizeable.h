/*
* GuiScrollViewAutoSizeable:
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