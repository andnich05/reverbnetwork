#include "GuiCustomSplashScreen.h"
#include "../vstgui4/vstgui/lib/cframe.h"

namespace VSTGUI{

	GuiCustomSplashScreen::GuiCustomSplashScreen(const CRect& size, CControlListener* listener, int32_t tag, CView* splashView)
	: CSplashScreen(size, listener, tag, splashView), savedUserData(nullptr) {

	}

	GuiCustomSplashScreen::~GuiCustomSplashScreen() {
		if (savedUserData) {
			delete savedUserData;
			savedUserData = nullptr;
		}
	}

	void GuiCustomSplashScreen::splash() {
		value = 1.0;
		if (modalView && getFrame() && getFrame()->setModalView(modalView))
		{
			CControl::valueChanged();
		}
	}
}