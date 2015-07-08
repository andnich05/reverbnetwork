#include "GuiGraphicsView.h"
#include "ValueConversion.h"
#include "GuiGraphicsModule.h"
#include "../vstgui4/vstgui/lib/cdrawcontext.h"
#include "../vstgui4/vstgui/lib/cbitmap.h"
#include <iomanip>
#include <sstream>

namespace VSTGUI {

	GuiGraphicsView::GuiGraphicsView(const CRect& size, const int& numberOfModules)
	: CViewContainer(size) {
		modules.resize(numberOfModules);
	}


	GuiGraphicsView::~GuiGraphicsView() {

	}

	void GuiGraphicsView::addModule(const std::string& title, const int& id, const CPoint& position, const std::vector<double>& inputGainValues, const std::vector<std::string>& inputNames) {
		GuiGraphicsModule* module = new GuiGraphicsModule(position, title, inputGainValues, inputNames);
		modules[id] = module;
	}

	void GuiGraphicsView::drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect)
	{
		if (getDrawBackground())
		{
			CRect oldClip;
			pContext->getClipRect(oldClip);
			CRect newClip(_updateRect);
			newClip.bound(oldClip);
			pContext->setClipRect(newClip);
			CRect tr(0, 0, getViewSize().getWidth(), getViewSize().getHeight());
			getDrawBackground()->draw(pContext, tr, backgroundOffset);
			pContext->setClipRect(oldClip);
		}
		else if ((backgroundColor.alpha != 255 && getTransparency()) || !getTransparency())
		{
			pContext->setDrawMode(kAliasing);
			pContext->setLineWidth(1);
			pContext->setFillColor(backgroundColor);
			pContext->setFrameColor(backgroundColor);
			pContext->setLineStyle(kLineSolid);
			CRect r;
			if (backgroundColorDrawStyle == kDrawFilled || (backgroundColorDrawStyle == kDrawFilledAndStroked && backgroundColor.alpha == 255))
			{
				r = _updateRect;
				r.inset(-1, -1);
			}
			else
			{
				r = getViewSize();
				r.offset(-r.left, -r.top);
			}
			pContext->drawRect(r, backgroundColorDrawStyle);
		}

		//redraw(pContext);
	}

	void GuiGraphicsView::redraw(CDrawContext* pContext) {
		drawModuleRects(pContext);
		drawModuleConnections(pContext);
	}

	void GuiGraphicsView::drawModuleRects(CDrawContext* pContext) {
		for (auto&& m : modules) {
			
		}


		//!!!!!!!!!!!!!!
		//rearrangeModules();
	}

	void GuiGraphicsView::drawModuleConnections(CDrawContext* pContext) {
		/*for (auto&& m : modules) {
			if (m.enabled) {
				for (unsigned int j = 0; j < m.inputGainValues.size(); ++j) {
					if (m.inputGainValues[j] != 0.0) {
						pContext->setFrameColor(CColor(255, 255, 255, (uint8_t)(255 * abs(m.inputGainValues[j]))));
						CPoint line[2];
						line[0] = m.inputRects[j].getCenter();
						line[1] = modules[j].outputRect.getCenter();
						pContext->drawLines(line, 2);
					}
				}
			}
		}*/
	}

	void GuiGraphicsView::rearrangeModules() {
		//int notUsedCounter = 0;
		//for (unsigned int m = 0; m < modules.size(); ++m) {
		//	if (modules[m].numberOfUsedInputs == 0) {
		//		modules[m].position.x = this->getWidth() - modules[m].completeRegion.getWidth();
		//		modules[m].position.y = 0 + notUsedCounter * modules[m].completeRegion.getHeight();
		//		++notUsedCounter;
		//		/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
		//		fprintf(pFile, "y(n): %s\n", std::to_string(m.completeRegion.getWidth()).c_str());
		//		fclose(pFile);*/
		//	}
		//	else {
		//		modules[m].position.x = (modules[m].completeRegion.getWidth() + 20) * m;
		//	}

		//	modules[m].completeRegion = CRect(CPoint(modules[m].position.x, (modules[m].position.y)), CPoint(modules[m].completeRegion.getWidth(), modules[m].completeRegion.getHeight()));
		//	modules[m].handleRegion = CRect(CPoint(modules[m].position.x, (modules[m].position.y)), CPoint(modules[m].handleRegion.getWidth(), modules[m].handleRegion.getHeight()));
		//}
		this->setDirty();
	}

	



}