/*
* GuiCustomSplashScreen: Added function to splash the view manually
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
		virtual inline void* getUserData() const { if (savedUserData) return savedUserData; else return nullptr; }
		// Delete the user data
		virtual inline void releaseUserData() { if (savedUserData) delete savedUserData; savedUserData = nullptr; }

	protected:
		void* savedUserData;
	};
}

#endif // GUICUSTOMSPLASHSCREEN_H