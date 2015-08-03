/***********************************************************************************************************************
**
** Copyright (c) 2015 ETH Zurich
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
** following conditions are met:
**
**    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
**      disclaimer.
**    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
**      following disclaimer in the documentation and/or other materials provided with the distribution.
**    * Neither the name of the ETH Zurich nor the names of its contributors may be used to endorse or promote products
**      derived from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
** INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
** WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
***********************************************************************************************************************/

#include "ArrowOverlay.h"
#include "utils/Drawing.h"

namespace Visualization {

ITEM_COMMON_DEFINITIONS(ArrowOverlay, "item")

ArrowOverlay::ArrowOverlay(Item* arrowFrom, Item* arrowTo, const StyleType* style)
	: Super{{arrowFrom, arrowTo}, style}
{
}

void ArrowOverlay::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	QPen pen; // Use default pen
	Drawing::drawArrow(painter, lineFrom_, lineTo_, pen, pen, false, true, 1);
}

void ArrowOverlay::determineChildren(){}

void ArrowOverlay::updateGeometry(int, int)
{
	// Find the space the line will occupy in the scene
	auto first = firstAssociatedItem();
	auto second = secondAssociatedItem();

	auto firstPos = first->scenePos();
	auto secondPos = second->scenePos();
	QPointF leftTop;
	QPointF rightBottom;
	if (firstPos.x() < secondPos.x())
	{
		if (firstPos.y() < secondPos.y())
		{
			// first is at top left
			leftTop = {firstPos.x() + first->widthInLocal(), firstPos.y() + first->heightInLocal() / 2};
			rightBottom = {secondPos.x(), secondPos.y() + second->heightInLocal() / 2};
			lineFrom_ = {0, 0};
			lineTo_ = rightBottom - leftTop;
		}
		else
		{
			// first is at bottom left
			leftTop = {firstPos.x() + first->widthInLocal(), secondPos.y() + second->heightInLocal() / 2};
			rightBottom = {secondPos.x(), firstPos.y() + first->heightInLocal() / 2};
			lineFrom_ = {0, rightBottom.y() - leftTop.y()};
			lineTo_ = {rightBottom.x() - leftTop.x(), 0};
		}
	}
	else
	{
		if (secondPos.y() < firstPos.y())
		{
			// second is at top left -> first is at bottom right
			leftTop = {secondPos.x() + second->widthInLocal(), secondPos.y() + second->heightInLocal() / 2};
			rightBottom = {firstPos.x(), firstPos.y() + first->heightInLocal() / 2};
			lineFrom_ = rightBottom - leftTop;
			lineTo_ = {0, 0};
		}
		else
		{
			// second is at bottom left -> first is at top right
			leftTop = {secondPos.x() + second->widthInLocal(), firstPos.y() + first->heightInLocal() / 2};
			rightBottom = {firstPos.x(), secondPos.y() + second->heightInLocal() / 2};
			lineFrom_ = {rightBottom.x() - leftTop.x(), 0};
			lineTo_ = {0, rightBottom.y() - leftTop.y()};
		}
	}

	setPos(leftTop);
	setSize(rightBottom.x() - leftTop.x(), rightBottom.y() - leftTop.y());
}

}
