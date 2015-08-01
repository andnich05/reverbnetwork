#ifndef GUICUSTOMSPLASHSCREEN_H
#define GUICUSTOMSPLASHSCREEN_H

#include "../vstgui4/vstgui/lib/controls/csplashscreen.h"

namespace VSTGUI {

	class GuiCustomSplashScreen : public CSplashScreen {
	public:
		GuiCustomSplashScreen(const CRect& size, CControlListener* listener, int32_t tag, CView* splashView);
		~GuiCustomSplashScreen();

		// Show the splash screen
		virtual void splash();
		// Save some user date
		virtual inline void setUserData(void* userData) { savedUserData = userData; }
		// Get the user data
		virtual inline void* getUserData() const { if(savedUserData) return savedUserData; }
		// Delete the user data
		virtual inline void releaseUserData() { if (savedUserData) delete savedUserData; savedUserData = nullptr; }

	protected:
		void* savedUserData;
	};
}

#endif // GUICUSTOMSPLASHSCREEN_H