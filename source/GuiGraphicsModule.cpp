#include "GuiGraphicsModule.h"
#include "ReverbNetworkDefines.h"
#include "../vstgui4/vstgui/lib/cdrawcontext.h"
#include "../vstgui4/vstgui/lib/cbitmap.h"
#include <sstream>
#include <iomanip>

namespace VSTGUI {

	const int inoutRectWidth = 8;
	const int inoutRectHeight = 10;
	const int spacing = 2;

	GuiGraphicsModule::GuiGraphicsModule(const std::string& title, const int& numberOfInputs, const bool& hasOutput)
		: CViewContainer(CRect(0, 0, 0, 0)), title(title), numberOfInputs(numberOfInputs), hasOutput(hasOutput) {

		//enabled = true;

		inputsEnabled.resize(numberOfInputs, false);
		inputGainValues.resize(numberOfInputs, 0.0);
		inputRects.resize(numberOfInputs, CRect(0, 0, 0, 0));

		mouseDownCoordinates = CPoint(0, 0);
		mouseUpCoordinates = CPoint(0, 0);
		mouseDownInHandleRegion = false;

		mouseDownInOutputRect = false;
		mouseMoveOutputRect = 0;

		clickedInput = 0;

		updateShape();
	}

	GuiGraphicsModule::~GuiGraphicsModule() {

	}

	void GuiGraphicsModule::setInputNames(const std::vector<std::string>& inputNames) {
		this->inputNames = inputNames;
		updateShape();
	}

	void GuiGraphicsModule::drawBackgroundRect(CDrawContext* pContext, const CRect& _updateRect)
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

		updateShape();
		redraw(pContext);
	}

	void GuiGraphicsModule::updateInput(const int& input, const double& gainValue) {
		if (input < numberOfInputs) {
			inputGainValues[input] = gainValue;
			if (gainValue == 0.0) {
				inputsEnabled[input] = false;
			}
			else {
				inputsEnabled[input] = true;
			}
			updateShape();
		}
	}

	void GuiGraphicsModule::clearInputs() {
		for (auto&& i : inputGainValues) {
			i = 0.0;
		}
		for (auto&& i : inputsEnabled) {
			i = false;
		}
		updateShape();
	}

	void GuiGraphicsModule::updateShape() {
		handleRegion = CRect(CPoint(0, 0), CPoint(100, 12));
		numberOfUsedInputs = 0;
		for (int i = 0; i < numberOfInputs; ++i) {
			if (inputsEnabled[i]) {
				inputRects[i] = CRect(CPoint(0, handleRegion.getHeight() + spacing).offset(0, numberOfUsedInputs * (spacing + inoutRectHeight)), CPoint(inoutRectWidth, inoutRectHeight));
				++numberOfUsedInputs;
			}
			else {
				inputRects[i] = 0;
			}
		}
		mainRegion = CRect(CPoint(0, handleRegion.getHeight()), CPoint(handleRegion.getWidth(), spacing + numberOfUsedInputs * (spacing + inoutRectHeight) + spacing));
		completeRegion = CRect(CPoint(0, 0), CPoint(handleRegion.getWidth(), handleRegion.getHeight() + mainRegion.getHeight()));
		this->setViewSize(CRect(CPoint(this->getViewSize().getTopLeft()), CPoint(completeRegion.getWidth(), completeRegion.getHeight())));
		this->setMouseableArea(this->getViewSize());
		outputRect = CRect(CPoint(handleRegion.getWidth() - inoutRectWidth, completeRegion.getCenter().y - inoutRectHeight / 2), CPoint(inoutRectWidth, inoutRectHeight));	
	}

	void GuiGraphicsModule::redraw(CDrawContext* pContext) {
		if (isVisible()) {
			pContext->setFrameColor(CColor(0, 0, 0));
			pContext->setFillColor(CColor(50, 50, 50));
			pContext->setLineWidth(1);
			pContext->setFont(kNormalFontSmaller);
			pContext->setFontColor(CColor(255, 255, 255));
			pContext->drawRect(handleRegion, CDrawStyle::kDrawFilledAndStroked); // title rect
			pContext->drawString(title.c_str(), CPoint(2, 10), false); // title string
			pContext->setFillColor(CColor(50, 50, 50));
			pContext->drawRect(mainRegion, kDrawFilledAndStroked); // main rect
			std::stringstream temp;
			for (int i = 0; i < inputRects.size(); ++i) {
				if (inputsEnabled[i]) {
					pContext->setFillColor(CColor(50, 200, 50));
					pContext->drawRect(inputRects[i], CDrawStyle::kDrawFilledAndStroked); // module inputs
					temp.str(std::string());
					temp.clear();
					temp << inputNames[i];
					if (inputGainValues[i] != 0.0) {
						temp << " [" << std::setprecision(2) << inputGainValues[i] << "]";
					}
					pContext->drawString(temp.str().c_str(), CRect(CPoint(inputRects[i].right + spacing, inputRects[i].top), CPoint(100, inoutRectHeight)), CHoriTxtAlign::kLeftText, false);
				}
			}
			if (hasOutput) {
				pContext->setFillColor(CColor(200, 200, 200));
				pContext->drawRect(outputRect, kDrawFilledAndStroked);
				pContext->drawString(outputName.c_str(), CRect(CPoint(outputRect.left - 50, outputRect.top), CPoint(50, outputRect.getHeight())), CHoriTxtAlign::kRightText, false);
			}
		}
	}

	CMouseEventResult GuiGraphicsModule::onMouseDown(CPoint &where, const CButtonState& buttons)
	{
		// Reference conversion!
		frameToLocal(where);

		// Start drawing new connection line at the output rect
		if (where.isInside(outputRect)) {
			if (!mouseDownInOutputRect) {
				getParentView()->notify(this, "StartMouseLine");
				mouseDownInOutputRect = true;
				return kMouseEventHandled;
			}
		}
		// Moving the modules around
		if (where.isInside(handleRegion)) {
			/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
			fprintf(pFile, "y(n): %s\n", "INSIDE");
			fclose(pFile);*/
			mouseDownInHandleRegion = true;
			mouseDownCoordinates = where;
			return kMouseEventHandled;
		}
		// Remove connection to an input
		for (unsigned int i = 0; i < inputRects.size(); ++i) {
			if (where.isInside(inputRects[i])) {
				inputGainValues[i] = 0.0;
				clickedInput = i;
				getParentView()->notify(this, "RemoveConnection");
				return kMouseEventHandled;
			}
		}

		return kMouseEventHandled;
	}

	CMouseEventResult GuiGraphicsModule::onMouseMoved(CPoint &where, const CButtonState& buttons)
	{	
		if (mouseDownInOutputRect) {
			this->mouseMoveOutputRect = where;
			getParentView()->notify(this, "MoveMouseLine");
			return kMouseEventHandled;
		}
		else if (mouseDownInHandleRegion) {
			if (where != mouseDownCoordinates) {
				this->setViewSize(CRect(CPoint(where - mouseDownCoordinates), CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
				this->setMouseableArea(this->getViewSize());
				this->getParentView()->setDirty();
				return kMouseEventHandled;
			}
		}
		return kMouseEventHandled;
	}

	CMouseEventResult GuiGraphicsModule::onMouseUp(CPoint& where, const CButtonState& buttons) {
		mouseDownInHandleRegion = false;
		
		if (mouseDownInOutputRect) {
			mouseDownInOutputRect = false;
			mouseUpCoordinates = where;
			getParentView()->notify(this, "EndMouseLine");
		}

		return kMouseEventHandled;
	}
}