#ifndef GUIOPENGLGRAPHICSVIEW_H
#define GUIOPENGLGRAPHICSVIEW_H

#include "../vstgui4/vstgui/lib/copenglview.h"

namespace VSTGUI {

	class GuiOpenGLGraphicsView :
		public COpenGLView
	{
	public:
		GuiOpenGLGraphicsView(const CRect& size);
		~GuiOpenGLGraphicsView();

		void drawOpenGL(const CRect& updateRect);
	};

}

#endif // GUIOPENGLGRAPHICSVIEW_H