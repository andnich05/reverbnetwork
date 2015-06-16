#include "GuiOpenGLGraphicsView.h"

namespace VSTGUI {

	GuiOpenGLGraphicsView::GuiOpenGLGraphicsView(const CRect& size) 
	: COpenGLView(size) {

		CRect r(getViewSize());

	}

	GuiOpenGLGraphicsView::~GuiOpenGLGraphicsView() {

	}

	void GuiOpenGLGraphicsView::drawOpenGL(const CRect& updateRect) {

	}
}
