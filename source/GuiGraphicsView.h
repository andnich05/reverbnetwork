#ifndef GUIGRAPHICSVIEW_H
#define GUIGRAPHICSVIEW_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"

namespace VSTGUI {

	class GuiGraphicsView :
		public CViewContainer
	{
	public:
		GuiGraphicsView(const CRect& size);
		~GuiGraphicsView();

		virtual void drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect);

		virtual void drawStuff(CDrawContext* pContext);
	};

}

#endif // GUIGRAPHICSVIEW_H