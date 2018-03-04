/*
* GuiBaseAPModule: Contains the module components
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

#ifndef GUIBASEAPMODULE_H
#define GUIBASEAPMODULE_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"

class GuiBaseAPModule :
	public VSTGUI::CViewContainer
{
public:
	// Handle region is the region where the user can drag the module by pressing and holding the mouse button
	GuiBaseAPModule(const VSTGUI::CRect& size, const VSTGUI::CRect& handleRegion, unsigned int moduleId, CBaseObject* editor);

	//--- Overrides from CViewContainer
	virtual VSTGUI::CMouseEventResult onMouseDown(VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons) override;
	virtual VSTGUI::CMouseEventResult onMouseMoved(VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons) override;
	virtual VSTGUI::CMouseEventResult onMouseUp(VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons) override;
	virtual void drawBackgroundRect(VSTGUI::CDrawContext* pContext, const VSTGUI::CRect& _updateRect) override;

	// Get the unique module id
	unsigned int getModuleId() const;
	// Collapse view so that only the handle is visible
	void collapseView(bool collapse);
	// Check if the view is collapsed
	inline bool isCollapsed() const { return collapsed; }

	static const char* kModuleWantsFocus; // Notify message when the user clicks on the module

private:
	double mousePressedX; // Mouse position x when pressed
	double mousePressedY; // Mouse position y when pressed
	bool mousePressed; // Is mouse pressed at the moment?
	VSTGUI::CPoint handleSize; // Size of the handle for dragging
	VSTGUI::CPoint viewSize; // Used for uncollapsing
	unsigned int moduleId; // Module identifier
	bool collapsed; // Is view collapsed?
	CBaseObject* editor; // VST Editor object (Pointer to ReverbNetworkEditor), used for notifying
};

#endif // GUIBASEAPMODULE_H