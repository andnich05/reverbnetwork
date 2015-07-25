#ifndef GUIGRAPHICSVIEW_H
#define GUIGRAPHICSVIEW_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"
#include <vector>

namespace VSTGUI {
	class GuiGraphicsModule;
	class GuiGraphicsConnections;

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

		
		virtual void createModule(const std::string& title, const int& id, const int& numberOfInputs);
		// Makes another module visible; returns false if the maximum number of modules is reached
		virtual bool addModule();
		virtual void createVstInput();
		virtual void createVstOutput();
		virtual void setModuleInputNames(const int& moduleId, const std::vector<std::string> inputNames);
		virtual void updateModule(const int& moduleId, const int& input, const double& gainValue);
		virtual void setVstOutputConnection(const int& vstOutput, const int& moduleOrVstInput);
		virtual void clearModules();
		//virtual void setModuleEnabled(const int& id, const bool& enabled);
		virtual void rearrangeModules();

		inline const Connection& getDrawnConnection() { return drawnConnection; }
		inline const int& getModuleClicked() { return moduleClicked; }

		// Overrides
		virtual void drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect);
		//virtual CMouseEventResult onMouseDown(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		//virtual CMouseEventResult onMouseUp(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		//virtual CMouseEventResult onMouseMoved(CPoint& where, const CButtonState& buttons) VSTGUI_OVERRIDE_VMETHOD;
		//virtual CMouseEventResult onMouseCancel() VSTGUI_OVERRIDE_VMETHOD;
		virtual CMessageResult notify(CBaseObject* sender, IdStringPtr message) VSTGUI_OVERRIDE_VMETHOD;

	private:
		std::vector<GuiGraphicsModule*> modules;
		std::vector<GuiGraphicsModule*> vstInputs;
		std::vector<GuiGraphicsModule*> vstOutputs;
		std::vector<int> vstOutputConnections;

		GuiGraphicsConnections* connections;

		virtual void redraw(CDrawContext* pContext);
		//virtual void drawModuleRects(CDrawContext* pContext);
		virtual void drawModuleConnections(CDrawContext* pContext);

		int numberOfModules;
		int numberOfInputs;

		CBaseObject* editor;

		Connection drawnConnection;

		int moduleClicked;

	};

}

#endif // GUIGRAPHICSVIEW_H