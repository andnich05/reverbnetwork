#ifndef GUIGRAPHICSVIEW_H
#define GUIGRAPHICSVIEW_H

#include "../vstgui4/vstgui/lib/cviewcontainer.h"
#include <vector>

namespace VSTGUI {
	class GuiGraphicsModule;
	class GuiGraphicsConnections;

	struct Connection {
		Connection(int source = 0, int destination = 0)
			: source(source), destination(destination) {}
		int source;
		int destination;
	};


	class GuiGraphicsView :
		public CViewContainer
	{
	public:
		GuiGraphicsView(const CRect& size, const int& numberOfModules, CBaseObject* editor);
		~GuiGraphicsView();

		
		virtual void addModule(const std::string& title, const int& id, const int& numberOfInputs);
		virtual void addVstInput();
		virtual void addVstOutput();
		virtual void setModuleInputNames(const int& moduleId, const std::vector<std::string> inputNames);
		virtual void updateModule(const int& moduleId, const int& input, const double& gainValue);
		virtual void clearModules();
		//virtual void setModuleEnabled(const int& id, const bool& enabled);
		virtual void rearrangeModules();

		inline const Connection& getDrawnConnection() { return drawnConnection; }

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

		GuiGraphicsConnections* connections;

		virtual void redraw(CDrawContext* pContext);
		//virtual void drawModuleRects(CDrawContext* pContext);
		virtual void drawModuleConnections(CDrawContext* pContext);

		int numberOfModules;
		int numberOfInputs;

		CBaseObject* editor;

		Connection drawnConnection;
	};

}

#endif // GUIGRAPHICSVIEW_H