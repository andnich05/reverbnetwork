/*
* GuiGraphicsView: Parent view for modules and connections
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

#ifndef GUIGRAPHICSVIEW_H
#define GUIGRAPHICSVIEW_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"
#include <vector>

namespace VSTGUI {
	class GuiGraphicsModule;
	class GuiGraphicsConnections;

	// Connection structure
	struct Connection {
		Connection(int source = 0, int destination = 0, double gainValue = 1.0)
			: source(source), destination(destination), gainValue(gainValue) {}
		int source;
		int destination;
		//bool establish; // True if establish, false if remove connection
		double gainValue;
	};

	class GuiGraphicsView :
		public CViewContainer
	{
	public:
		GuiGraphicsView(const CRect& size, const int& numberOfModules, CBaseObject* editor);
		~GuiGraphicsView();

		// Create new module view
		virtual void createModule(const std::string& title, const int& id, const int& numberOfInputs);
		// Make the next module visible which is not visible yet; returns false if the maximum number of modules is reached
		virtual bool addModule();
		// Make a specific module visible
		virtual void makeModuleVisible(const int& moduleId, const bool& visible);
		// Check if a module is visible
		virtual bool isModuleVisible(const int& moduleId);
		// Create new vst input view
		virtual void createVstInput();
		// Create new vst output view
		virtual void createVstOutput();
		// Set the names of the inputs of a module
		virtual void setModuleInputNames(const int& moduleId, const std::vector<std::string> inputNames);
		// Update a module
		virtual void updateModule(const int& moduleId, const int& input, const double& gainValue);
		// Create a new connection from a module or a vst input to a vst output
		virtual void setVstOutputConnection(const int& vstOutput, const int& moduleOrVstInput);
		// Clear all outputs of a module
		virtual void clearModules();
		// (Try to) rearrange modules...
		virtual void rearrangeModules();
		// Change module position
		virtual void setModulePosition(const int& moduleId, const CPoint& position);
		// Get module position
		virtual CPoint getModulePosition(const int& moduleId) const;
		// Change Vst input position
		virtual void setVstInputPosition(const int& vstInput, const CPoint& position);
		// Get Vst input position
		virtual CPoint getVstInputPosition(const int& vstInput) const;
		// Change Vst output position
		virtual void setVstOutputPosition(const int& vstOutput, const CPoint& position);
		// Get Vst input position
		virtual CPoint getVstOutputPosition(const int& vstOutput) const;
		// Get a new drawn connection in order to update the other GUI elements
		inline const Connection& getDrawnConnection() { return drawnConnection; }
		// Get the module the user double clicked on
		inline const int& getModuleClicked() { return moduleClicked; }
		// Set a module title
		void setModuleTitle(const int& moduleId, const std::string& title);

		// Overrides from CViewContainer
		virtual void drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect);
		virtual CMessageResult notify(CBaseObject* sender, IdStringPtr message) VSTGUI_OVERRIDE_VMETHOD;

	private:
		std::vector<GuiGraphicsModule*> modules; // Module views
		std::vector<GuiGraphicsModule*> vstInputs; // Vst input views
		std::vector<GuiGraphicsModule*> vstOutputs; // Vst output views
		std::vector<int> vstOutputConnections; // Connections to Vst outputs

		GuiGraphicsConnections* connections;

		// Redraw everything
		virtual void redraw(CDrawContext* pContext);
		// Draw connections
		virtual void drawModuleConnections(CDrawContext* pContext);

		int numberOfModules;
		CBaseObject* editor; // Pointer to ReverbNetworkEditor
		Connection drawnConnection;
		int moduleClicked;
	};

}

#endif // GUIGRAPHICSVIEW_H