#ifndef GUICUSTOMSPLASHSCREEN_H
#define GUICUSTOMSPLASHSCREEN_H

#include "../vstgui4/vstgui/lib/controls/csplashscreen.h"

namespace VSTGUI {

	class GuiCustomSplashScreen :
		public CSplashScreen
	{
	public:
		GuiCustomSplashScreen(const CRect& size, CControlListener* listener, int32_t tag, CView* splashView);
		~GuiCustomSplashScreen();

		// Show the splash screen
		virtual void splash();
		virtual inline void setUserData(void* userData) { savedUserData = userData; }
		virtual inline void* getUserData() const { if(savedUserData) return savedUserData; }
		virtual inline void releaseUserData() { if (savedUserData) delete savedUserData; savedUserData = nullptr; }

		virtual CMouseEventResult onMouseDown(CPoint& where, const CButtonState& buttons);
	
	protected:
		void* savedUserData;
	};
}

#endif // GUICUSTOMSPLASHSCREEN_H