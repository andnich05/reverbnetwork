#include "GuiGraphicsModule.h"
#include "ReverbNetworkDefines.h"
#include "../vstgui4/vstgui/lib/cdrawcontext.h"
#include "../vstgui4/vstgui/lib/cbitmap.h"
#include "../vstgui4/vstgui/lib/cframe.h"
#include <sstream>
#include <iomanip>
#include "ValueConversion.h"

namespace VSTGUI {

#ifndef FOREACHSUBVIEW_REVERSE
#define FOREACHSUBVIEW_REVERSE(reverse) ChildViewConstIterator it; ChildViewConstReverseIterator rit; if (reverse) rit = children.rbegin (); else it = children.begin (); while (reverse ? rit != children.rend () : it != children.end ()) { CView* pV; if (reverse) {	pV = (*rit); rit++; } else { pV = (*it); it++; } {
#endif

	const int inoutRectWidth = 8;
	const int inoutRectHeight = 10;
	const int spacing = 2;

	GuiGraphicsModule::GuiGraphicsModule(const std::string& title, const int& numberOfInputs, const bool& hasOutput, const bool& hasGainValue)
		: CViewContainer(CRect(0, 0, 0, 0)), title(title), numberOfInputs(numberOfInputs), hasOutput(hasOutput) {

		//enabled = true;

		inputsEnabled.resize(numberOfInputs, false);
		inputGainValues.resize(numberOfInputs, 0.0);
		inputRects.resize(numberOfInputs, CRect(0, 0, 0, 0));
		
		for (int i = 0; i < numberOfInputs; ++i) {
			CTextLabel* textLabelInputName = new CTextLabel(CRect(CPoint(0, 0), CPoint(50, 12)), "");
			textLabelInputName->setBackColor(CColor(0, 0, 0, 0));
			textLabelInputName->setFrameColor(CColor(0, 0, 0, 0));
			textLabelInputName->setFont(kNormalFontSmaller);
			textLabelInputName->setAntialias(false);
			this->addView(textLabelInputName);
			textLabelInputName->setVisible(false);
			inputNames.push_back(textLabelInputName);

			if (hasGainValue) {
				CTextEdit* textEditGainValue = new CTextEdit(CRect(CPoint(0, 0), CPoint(30, 12)), this, i, "");
				textEditGainValue->setBackColor(CColor(0, 0, 0, 0));
				textEditGainValue->setFrameColor(CColor(0, 0, 0, 0));
				textEditGainValue->setAntialias(false);
				textEditGainValue->setFont(kNormalFontSmaller);
				textEditGainValue->setMin(MIN_MIXERGAIN);
				textEditGainValue->setMax(MAX_MIXERGAIN);
				textEditGainValue->setStringToValueProc(ValueConversion::textEditStringToValueConversion);
				textEditGainValue->setValueToStringProc(ValueConversion::textEditValueToStringConversion);
				this->addView(textEditGainValue);
				textEditGainValue->setVisible(false);
				textEditsGainValues.push_back(textEditGainValue);
			}
		}

		mouseDownCoordinates = CPoint(0, 0);
		mouseUpCoordinates = CPoint(0, 0);
		mouseDownInHandleRegion = false;

		mouseDownInOutputRect = false;
		mouseMoveOutputRect = 0;

		inputToUpdate = 0;

		updateShape();
	}

	GuiGraphicsModule::~GuiGraphicsModule() {
		textEditsGainValues.clear();
	}

	void GuiGraphicsModule::setInputNames(const std::vector<std::string>& inputNames) {
		for (unsigned int i = 0; i < inputNames.size(); ++i) {
			this->inputNames[i]->setText(inputNames[i].c_str());
			this->inputNames[i]->sizeToFit();
		}
		
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
			if (textEditsGainValues.size() > 0) {
				textEditsGainValues[input]->setValue(gainValue);
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
			pContext->setFrameColor(CColor(0, 0, 0, 0));
			pContext->setFillColor(CColor(50, 50, 50));
			pContext->setLineWidth(1);
			pContext->setFont(kNormalFontSmaller);
			pContext->setFontColor(CColor(255, 255, 255));
			pContext->drawRect(handleRegion, CDrawStyle::kDrawFilledAndStroked); // title rect
			pContext->drawString(title.c_str(), CPoint(2, 10), false); // title string
			pContext->setFillColor(CColor(50, 50, 50));
			pContext->drawRect(mainRegion, kDrawFilledAndStroked); // main rect
			pContext->setFrameColor(CColor(0, 0, 0));
			pContext->setFillColor(CColor(0, 0, 0, 0));
			pContext->drawRect(completeRegion); // Frame
			std::stringstream temp;
			for (int i = 0; i < inputRects.size(); ++i) {
				if (inputsEnabled[i]) {
					pContext->setFillColor(CColor(50, 200, 50));
					pContext->drawRect(inputRects[i], CDrawStyle::kDrawFilledAndStroked); // module inputs
					inputNames[i]->setViewSize(CRect(CPoint(inputRects[i].right + spacing, inputRects[i].top), CPoint(inputNames[i]->getWidth(), inputNames[i]->getHeight())));
					inputNames[i]->setVisible(true);
					if (textEditsGainValues.size() > 0) {
						textEditsGainValues[i]->setViewSize(CRect(CPoint(inputNames[i]->getViewSize().right + spacing, inputRects[i].top), CPoint(textEditsGainValues[i]->getWidth(), textEditsGainValues[i]->getHeight())));
						textEditsGainValues[i]->setMouseableArea(textEditsGainValues[i]->getViewSize());
						textEditsGainValues[i]->setVisible(true);
					}
					/*temp.str(std::string());
					temp.clear();
					temp << inputNames[i];
					if (inputGainValues[i] != 0.0) {
						temp << " [" << std::setprecision(2) << inputGainValues[i] << "]";
					}
					pContext->drawString(temp.str().c_str(), CRect(CPoint(inputRects[i].right + spacing, inputRects[i].top), CPoint(100, inoutRectHeight)), CHoriTxtAlign::kLeftText, false);*/
				}
				else {
					inputNames[i]->setVisible(false);
					if (textEditsGainValues.size() > 0) {
						textEditsGainValues[i]->setVisible(false);
					}
				}
			}
			if (hasOutput) {
				pContext->setFillColor(CColor(200, 200, 200));
				pContext->drawRect(outputRect, kDrawFilledAndStroked);
				pContext->drawString(outputName.c_str(), CRect(CPoint(outputRect.left - 50, outputRect.top), CPoint(50, outputRect.getHeight())), CHoriTxtAlign::kRightText, false);
			}
		}
	}

	void GuiGraphicsModule::valueChanged(VSTGUI::CControl* pControl) {
		for (unsigned int i = 0; i < textEditsGainValues.size(); ++i) {
			if (textEditsGainValues[i] == pControl) {
				inputToUpdate = i;
				inputGainValues[i] = textEditsGainValues[i]->getValue();
				getParentView()->notify(this, "UpdateGainValue");
			}
		}

	}

	CMouseEventResult GuiGraphicsModule::onMouseDown(CPoint &where, const CButtonState& buttons)
	{
		if (buttons.isDoubleClick()) {
			getParentView()->notify(this, "DoubleClick");
			return kMouseEventHandled;
		}

		// Reference conversion!
		CPoint whereLocal(where);
		frameToLocal(whereLocal);

		// Start drawing new connection line at the output rect
		if (whereLocal.isInside(outputRect)) {
			if (!mouseDownInOutputRect) {
				getParentView()->notify(this, "StartMouseLine");
				mouseDownInOutputRect = true;
				return kMouseEventHandled;
			}
		}
		// Moving the modules around
		if (whereLocal.isInside(handleRegion)) {
			/*FILE* pFile = fopen("C:\\Users\\Andrej\\logVst.txt", "a");
			fprintf(pFile, "y(n): %s\n", "INSIDE");
			fclose(pFile);*/
			mouseDownInHandleRegion = true;
			mouseDownCoordinates = whereLocal;
			return kMouseEventHandled;
		}
		// Remove connection to an input
		for (unsigned int i = 0; i < inputRects.size(); ++i) {
			if (whereLocal.isInside(inputRects[i])) {
				inputGainValues[i] = 0.0;
				inputToUpdate = i;
				getParentView()->notify(this, "UpdateGainValue");
				return kMouseEventHandled;
			}
		}

		// convert to relativ pos
		CPoint where2(where);
		where2.offset(-getViewSize().left, -getViewSize().top);

		FOREACHSUBVIEW_REVERSE(true)
		//	FILE* pFile = fopen("E:\\logVst.txt", "a");
		//fprintf(pFile, "y(n): %s\n", "INSIDE");
		//fclose(pFile);
			if (pV && pV->isVisible() && pV->getMouseEnabled() && pV->hitTest(where2, buttons))
			{
			CControl* control = dynamic_cast<CControl*> (pV);
			if (control && control->getListener() && buttons & (kAlt | kShift | kControl | kApple | kRButton))
			{
				if (control->getListener()->controlModifierClicked((CControl*)pV, buttons) != 0)
					return kMouseEventHandled;
			}
			CBaseObjectGuard crg(pV);

			if (pV->wantsFocus())
				getFrame()->setFocusView(pV);

			CMouseEventResult result = pV->onMouseDown(where2, buttons);
			if (result != kMouseEventNotHandled && result != kMouseEventNotImplemented)
			{
				if (pV->getNbReference() > 1 && result == kMouseEventHandled)
					mouseDownView = pV;
				return result;
			}
			if (!pV->getTransparency())
				return result;
			}
		ENDFOREACHSUBVIEW
			return kMouseEventNotHandled;
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

				// Don't paint the modules outside the parent view
				if (this->getViewSize().getBottomRight().y > getParentView()->getViewSize().getBottomRight().y) {
					this->setViewSize(CRect(CPoint(getViewSize().getTopLeft().x, getParentView()->getViewSize().getBottomRight().y - getViewSize().getHeight()), CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
				}
				if (this->getViewSize().getBottomRight().x > getParentView()->getViewSize().getBottomRight().x) {
					this->setViewSize(CRect(CPoint(getParentView()->getViewSize().getBottomRight().x - getViewSize().getWidth(), getViewSize().getTopLeft().y), CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
				}
				if (this->getViewSize().getTopLeft().x < 0.0) {
					this->setViewSize(CRect(CPoint(0, getViewSize().getTopLeft().y), CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
				}
				if (this->getViewSize().getTopLeft().y < 0.0) {
					this->setViewSize(CRect(CPoint(getViewSize().getTopLeft().x, 0), CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
				}

				return kMouseEventHandled;
			}
		}
		
		if (mouseDownView)
		{
			CBaseObjectGuard crg(mouseDownView);

			// convert to relativ pos
			CPoint where2(where);
			where2.offset(-getViewSize().left, -getViewSize().top);
			CMouseEventResult mouseResult = mouseDownView->onMouseMoved(where2, buttons);
			if (mouseResult != kMouseEventHandled && mouseResult != kMouseEventNotImplemented)
			{
				mouseDownView = 0;
				return kMouseEventNotHandled;
			}
			return kMouseEventHandled;
		}
		return kMouseEventNotHandled;
	}

	CMouseEventResult GuiGraphicsModule::onMouseUp(CPoint& where, const CButtonState& buttons) {
		mouseDownInHandleRegion = false;
		
		if (mouseDownInOutputRect) {
			mouseDownInOutputRect = false;
			mouseUpCoordinates = where;
			getParentView()->notify(this, "EndMouseLine");
			return kMouseEventHandled;
		}

		if (mouseDownView)
		{
			CBaseObjectGuard crg(mouseDownView);

			// convert to relativ pos
			CPoint where2(where);
			where2.offset(-getViewSize().left, -getViewSize().top);
			mouseDownView->onMouseUp(where2, buttons);
			mouseDownView = 0;
			return kMouseEventHandled;
		}
		return kMouseEventNotHandled;
	}
}