/*
* GuiGraphicsModule: Paint a module or Vst input and output
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

#include "../include/GuiGraphicsModule.h"

#include "../include/ReverbNetworkDefines.h"
#include "../vstgui4/vstgui/lib/cdrawcontext.h"
#include "../vstgui4/vstgui/lib/cbitmap.h"
#include "../vstgui4/vstgui/lib/cframe.h"
#include <iomanip>
#include "../include/ValueConversion.h"

const int inoutRectWidth = 8;
const int inoutRectHeight = 10;
const int spacing = 2;

GuiGraphicsModule::GuiGraphicsModule(const std::string& title, const int& numberOfInputs, const bool& hasOutput, const bool& hasGainValue)
	: CViewContainer(VSTGUI::CRect(0, 0, 0, 0))
	, title(title)
	, completeRegion(VSTGUI::CRect(0, 0, 0, 0))
	, handleRegion(VSTGUI::CRect(0, 0, 0, 0))
	, mainRegion(VSTGUI::CRect(0, 0, 0, 0))
	, numberOfInputs(numberOfInputs)
	, numberOfUsedInputs(0)
	, hasOutput(hasOutput)
	, outputRect(VSTGUI::CRect(0, 0, 0, 0))
	, mouseDownCoordinates(VSTGUI::CPoint(0, 0))
	, mouseDownInHandleRegion(false)
	, mouseDownInOutputRect(false)
	, mouseMoveOutputRect(VSTGUI::CPoint(0, 0))
	, mouseUpCoordinates(VSTGUI::CPoint(0, 0))
	, inputToUpdate(0) {

	inputsEnabled.resize(numberOfInputs, false);
	inputGainValues.resize(numberOfInputs, 0.0);
	inputRects.resize(numberOfInputs, VSTGUI::CRect(0, 0, 0, 0));
		
	for (int i = 0; i < numberOfInputs; ++i) {
		VSTGUI::CTextLabel* textLabelInputName = new VSTGUI::CTextLabel(VSTGUI::CRect(VSTGUI::CPoint(0, 0), VSTGUI::CPoint(50, 12)), "");
		textLabelInputName->setBackColor(VSTGUI::CColor(0, 0, 0, 0));
		textLabelInputName->setFrameColor(VSTGUI::CColor(0, 0, 0, 0));
		textLabelInputName->setFont(VSTGUI::kNormalFontSmaller);
		textLabelInputName->setAntialias(false);
		this->addView(textLabelInputName);
		textLabelInputName->setVisible(false);
		inputNames.push_back(textLabelInputName);

		if (hasGainValue) {
			VSTGUI::CTextEdit* textEditGainValue = new VSTGUI::CTextEdit(VSTGUI::CRect(VSTGUI::CPoint(0, 0), VSTGUI::CPoint(30, 12)), this, i, "");
			textEditGainValue->setBackColor(VSTGUI::CColor(0, 0, 0, 0));
			textEditGainValue->setFrameColor(VSTGUI::CColor(0, 0, 0, 0));
			textEditGainValue->setAntialias(false);
			textEditGainValue->setFont(VSTGUI::kNormalFontSmaller);
			textEditGainValue->setMin(MIN_MIXERGAIN);
			textEditGainValue->setMax(MAX_MIXERGAIN);
			textEditGainValue->setStringToValueFunction(ValueConversion::textEditStringToValueConversion);
			textEditGainValue->setValueToStringFunction(ValueConversion::textEditValueToStringConversion);
			this->addView(textEditGainValue);
			textEditGainValue->setVisible(false);
			textEditsGainValues.push_back(textEditGainValue);
		}
	}

	updateShape();
}

GuiGraphicsModule::~GuiGraphicsModule() {
}

void GuiGraphicsModule::setInputNames(const std::vector<std::string>& inputNames) {
	for (unsigned int i = 0; i < inputNames.size(); ++i) {
		this->inputNames[i]->setText(inputNames[i].c_str());
		this->inputNames[i]->sizeToFit();
	}
	updateShape();
}

void GuiGraphicsModule::drawBackgroundRect(VSTGUI::CDrawContext* pContext, const VSTGUI::CRect& _updateRect)
{
	CViewContainer::drawBackgroundRect(pContext, _updateRect);

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
			textEditsGainValues[input]->setValue((float)gainValue);
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
	handleRegion = VSTGUI::CRect(VSTGUI::CPoint(0, 0), VSTGUI::CPoint(100, 12));
	numberOfUsedInputs = 0;
	for (int i = 0; i < numberOfInputs; ++i) {
		if (inputsEnabled[i]) {
			inputRects[i] = VSTGUI::CRect(VSTGUI::CPoint(0, handleRegion.getHeight() + spacing).offset(0, numberOfUsedInputs * (spacing + inoutRectHeight)), VSTGUI::CPoint(inoutRectWidth, inoutRectHeight));
			++numberOfUsedInputs;
		}
		else {
			inputRects[i] = VSTGUI::CRect(0, 0, 0, 0);
		}
	}
	mainRegion = VSTGUI::CRect(VSTGUI::CPoint(0, handleRegion.getHeight()), VSTGUI::CPoint(handleRegion.getWidth(), spacing + numberOfUsedInputs * (spacing + inoutRectHeight) + spacing));
	completeRegion = VSTGUI::CRect(VSTGUI::CPoint(0, 0), VSTGUI::CPoint(handleRegion.getWidth(), handleRegion.getHeight() + mainRegion.getHeight()));
	this->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(this->getViewSize().getTopLeft()), VSTGUI::CPoint(completeRegion.getWidth(), completeRegion.getHeight())));
	this->setMouseableArea(this->getViewSize());
	outputRect = VSTGUI::CRect(VSTGUI::CPoint(handleRegion.getWidth() - inoutRectWidth, completeRegion.getCenter().y - inoutRectHeight / 2), VSTGUI::CPoint(inoutRectWidth, inoutRectHeight));
}

void GuiGraphicsModule::redraw(VSTGUI::CDrawContext* pContext) {
	if (isVisible()) {
		pContext->setFrameColor(VSTGUI::CColor(0, 0, 0, 0));
		pContext->setFillColor(CCOLOR_GRAPHICSVIEW_MODULEHANDLE);
		pContext->setLineWidth(1);
		pContext->setFont(VSTGUI::kNormalFontSmaller);
		pContext->setFontColor(VSTGUI::CColor(255, 255, 255));
		pContext->drawRect(handleRegion, VSTGUI::CDrawStyle::kDrawFilledAndStroked); // title rect
		pContext->drawString(title.c_str(), VSTGUI::CPoint(2, 10), false); // title string
		pContext->setFillColor(CCOLOR_GRAPHICSVIEW_MODULEBACKGROUND);
		pContext->drawRect(mainRegion, VSTGUI::kDrawFilledAndStroked); // main rect
		pContext->setFrameColor(CCOLOR_MODULE_MAINFRAME);
		pContext->setFillColor(VSTGUI::CColor(0, 0, 0, 0));
		pContext->drawRect(completeRegion); // Frame
		for (unsigned int i = 0; i < inputRects.size(); ++i) {
			if (inputsEnabled[i]) {
				pContext->setFillColor(VSTGUI::CColor(0, 150, 0));
				pContext->drawRect(inputRects[i], VSTGUI::CDrawStyle::kDrawFilledAndStroked); // module inputs
				inputNames[i]->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(inputRects[i].right + spacing, inputRects[i].top), VSTGUI::CPoint(inputNames[i]->getWidth(), inputNames[i]->getHeight())));
				inputNames[i]->setVisible(true);
				if (textEditsGainValues.size() > 0) {
					textEditsGainValues[i]->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(inputNames[i]->getViewSize().right + spacing, inputRects[i].top), VSTGUI::CPoint(textEditsGainValues[i]->getWidth(), textEditsGainValues[i]->getHeight())));
					textEditsGainValues[i]->setMouseableArea(textEditsGainValues[i]->getViewSize());
					textEditsGainValues[i]->setVisible(true);
				}
			}
			else {
				inputNames[i]->setVisible(false);
				if (textEditsGainValues.size() > 0) {
					textEditsGainValues[i]->setVisible(false);
				}
			}
		}
		if (hasOutput) {
			pContext->setFillColor(VSTGUI::CColor(200, 50, 50));
			pContext->drawRect(outputRect, VSTGUI::kDrawFilledAndStroked);
			pContext->drawString(outputName.c_str(), VSTGUI::CRect(VSTGUI::CPoint(outputRect.left - 50, outputRect.top), VSTGUI::CPoint(50, outputRect.getHeight())), VSTGUI::CHoriTxtAlign::kRightText, false);
		}
	}
}

void GuiGraphicsModule::valueChanged(VSTGUI::CControl* pControl) {
	for (unsigned int i = 0; i < textEditsGainValues.size(); ++i) {
		if (textEditsGainValues[i] == pControl) {
			// If the user enters a new gain value => notify the parent view
			inputToUpdate = i;
			inputGainValues[i] = textEditsGainValues[i]->getValue();
			getParentView()->notify(this, "UpdateGainValue");
		}
	}

}

VSTGUI::CMouseEventResult GuiGraphicsModule::onMouseDown(VSTGUI::CPoint &where, const VSTGUI::CButtonState& buttons)
{
	if (this->isVisible()) {
		// Double click opens the module detail view
		if (buttons.isDoubleClick()) {
			getParentView()->notify(this, "DoubleClick");
			return VSTGUI::kMouseEventHandled;
		}

		// Reference conversion!
		VSTGUI::CPoint whereLocal(where);
		frameToLocal(whereLocal);

		// Start drawing new connection line at the output rect
		if (outputRect.pointInside(whereLocal)) {
			if (!mouseDownInOutputRect) {
				getParentView()->notify(this, "StartMouseLine");
				mouseDownInOutputRect = true;
				return VSTGUI::kMouseEventHandled;
			}
		}
		// Moving the modules around
		if (handleRegion.pointInside(whereLocal)) {
			mouseDownInHandleRegion = true;
			mouseDownCoordinates = whereLocal;
			return VSTGUI::kMouseEventHandled;
		}
		// Remove connection to an input when user clicks inside the input
		for (unsigned int i = 0; i < inputRects.size(); ++i) {
			if (inputRects[i].pointInside(whereLocal)) {
				inputGainValues[i] = 0.0;
				inputToUpdate = i;
				getParentView()->notify(this, "UpdateGainValue");
				return VSTGUI::kMouseEventHandled;
			}
		}
	}

	return CViewContainer::onMouseDown(where, buttons);
}

VSTGUI::CMouseEventResult GuiGraphicsModule::onMouseMoved(VSTGUI::CPoint &where, const VSTGUI::CButtonState& buttons)
{	
	if (this->isVisible()) {
		// Draw a new line when the user moves the mouse and has pressed it inside an output
		if (mouseDownInOutputRect) {
			this->mouseMoveOutputRect = where;
			getParentView()->notify(this, "MoveMouseLine");
			return VSTGUI::kMouseEventHandled;
		}
		// Move the view around
		else if (mouseDownInHandleRegion) {
			if (where != mouseDownCoordinates) {
				this->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(where - mouseDownCoordinates), VSTGUI::CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
				this->setMouseableArea(this->getViewSize());
				this->getParentView()->setDirty();

				// Don't paint the modules outside the parent view
				if (this->getViewSize().getBottomRight().y > getParentView()->getViewSize().getBottomRight().y) {
					this->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(getViewSize().getTopLeft().x, getParentView()->getViewSize().getBottomRight().y - getViewSize().getHeight()), VSTGUI::CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
				}
				if (this->getViewSize().getBottomRight().x > getParentView()->getViewSize().getBottomRight().x) {
					this->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(getParentView()->getViewSize().getBottomRight().x - getViewSize().getWidth(), getViewSize().getTopLeft().y), VSTGUI::CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
				}
				if (this->getViewSize().getTopLeft().x < 0.0) {
					this->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(0, getViewSize().getTopLeft().y), VSTGUI::CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
				}
				if (this->getViewSize().getTopLeft().y < 0.0) {
					this->setViewSize(VSTGUI::CRect(VSTGUI::CPoint(getViewSize().getTopLeft().x, 0), VSTGUI::CPoint(this->getViewSize().getWidth(), this->getViewSize().getHeight())));
				}

				return VSTGUI::kMouseEventHandled;
			}
		}
	}
		
	return CViewContainer::onMouseMoved(where, buttons);
}

VSTGUI::CMouseEventResult GuiGraphicsModule::onMouseUp(VSTGUI::CPoint& where, const VSTGUI::CButtonState& buttons) {
	mouseDownInHandleRegion = false;
		
	// Finish the mouse line
	if (this->isVisible()) {
		if (mouseDownInOutputRect) {
			mouseDownInOutputRect = false;
			mouseUpCoordinates = where;
			getParentView()->notify(this, "EndMouseLine");
			return VSTGUI::kMouseEventHandled;
		}
	}

	return CViewContainer::onMouseUp(where, buttons);
}
