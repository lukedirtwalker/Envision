/***********************************************************************************************************************
 * GridLayout.cpp
 *
 *  Created on: Mar 21, 2011
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#include "layouts/GridLayout.h"
#include "shapes/Shape.h"
#include "items/ItemWithNode.h"
#include "ModelRenderer.h"

#include "ModelBase/headers/nodes/Node.h"

namespace Visualization {

ITEM_COMMON_DEFINITIONS( GridLayout, "layout")

GridLayout::GridLayout(Item* parent, const StyleType* style) :
	Layout(parent, style), sizeX_(0), sizeY_(0)
{
}

GridLayout::~GridLayout()
{
	for (int x = 0; x<sizeX_; x++)
		for (int y = 0; y<sizeY_; y++) SAFE_DELETE_ITEM( items_[x][y] );
}

void GridLayout::setGridSize(int sizeX, int sizeY, bool deleteExtraItems)
{
	if (sizeX < sizeX_)
	{
		for(int x = sizeX; x < sizeX_; x++)
			for(int y = 0; y < sizeY_; y++)
				if (deleteExtraItems) SAFE_DELETE_ITEM( items_[x][y] );
				else if ( items_[x][y] ) items_[x][y]->setParentItem(NULL);
	}

	sizeX_ = sizeX;
	items_.resize(sizeX_);

	for (int x = 0; x < sizeX_; ++x)
	{
		if (sizeY < sizeY_ )
		{
			for(int y = sizeY; y<items_[x].size(); ++y)
				if (deleteExtraItems) SAFE_DELETE_ITEM( items_[x][y] );
				else if ( items_[x][y] ) items_[x][y]->setParentItem(NULL);
		}

		items_[x].resize(sizeY);
	}

	sizeY_ = sizeY;
}

bool GridLayout::isEmpty(int x, int y) const
{
	return x>= sizeX_ || y >= sizeY_ || items_[x][y] == NULL;
}

void GridLayout::set(Item* item, int x, int y, bool deleteOldItem)
{
	if(item) item->setParentItem(this);
	if (deleteOldItem) SAFE_DELETE_ITEM( items_[x][y]);
	else if (items_[x][y]) items_[x][y]->setParentItem(NULL);
	items_[x][y] = item;
	setUpdateNeeded();
}

void GridLayout::swap(int x1, int y1, int x2, int y2)
{
	Item* t = items_[x1][y1];
	items_[x1][y1] = items_[x2][y2];
	items_[x2][y2] = t;
	setUpdateNeeded();
}

void GridLayout::remove(int x, int y, bool deleteItem_)
{
	if (deleteItem_) SAFE_DELETE_ITEM( items_[x][y]);
	else if(items_[x][y]) items_[x][y]->setParentItem(NULL);
	items_[x][y] = NULL;
	setUpdateNeeded();
}

void GridLayout::remove(Item* item, bool deleteItem)
{
	if (item == NULL) return;

	for (int x = 0; x<sizeX_; ++x)
		for(int y = 0; y<sizeY_; ++y)
			if (items_[x][y] == item) items_[x][y] = NULL;

	if (deleteItem) SAFE_DELETE_ITEM(item);
	else item->setParentItem(NULL);
	setUpdateNeeded();
}

void GridLayout::clear(bool deleteItems)
{
	for (int x = 0; x<sizeX_; ++x)
		for(int y = 0; y<sizeY_; ++y)
		{
			if (deleteItems) SAFE_DELETE_ITEM(items_[x][y]);
			else if (items_[x][y]) items_[x][y]->setParentItem(NULL);

			items_[x][y] = NULL;
		}
	setUpdateNeeded();
}

void GridLayout::synchronizeWithNodes(const QList< QList<Model::Node*> >& nodes, ModelRenderer* renderer)
{
	// Remove all elements from the current grid that do not match a node
	// If a node exists record it's location
	QList<Model::Node*> nodesFound;
	QList<Item*> itemsFound;

	for (int x = 0; x < sizeX_; ++x)
		for (int y = 0; y < sizeY_; ++y)
			if (items_[x][y] != NULL)
			{
				bool found = false;

				for (int n = 0; n < nodes.size(); ++n)
					if (nodes[n].contains(items_[x][y]->node()))
					{
						found = true;
						nodesFound.append(items_[x][y]->node());
						itemsFound.append(items_[x][y]);
						break;
					}

				if (!found) SAFE_DELETE_ITEM(items_[x][y]);
				items_[x][y] = NULL;
			}

	// Get the new size.
	int newY = nodes.size();
	int newX = 0;
	for (int y = 0; y<newY; ++y)
		if (newX < nodes[y].size()) newX = nodes[y].size();
	setGridSize(newX, newY, false);

	for (int y = 0; y<nodes.size(); ++y)
		for (int x = 0; x<nodes[y].size(); ++x)
		{
			int oldIndex = nodesFound.indexOf(nodes[y][x]);
			if (oldIndex >=0) set(itemsFound[oldIndex], x, y, false);
			else set(renderer->render(NULL, nodes[y][x]), x, y, false);
		}
}


void GridLayout::synchronize(Item*& item, Model::Node* node, int x, int y)
{
	if (item && item->node() != node )
	{
		remove(item);
		item = NULL;
	}

	if (!item && node)
	{
		item = renderer()->render(NULL, node);
		set(item, x, y, true);
	}

}

bool GridLayout::isEmpty() const
{
	for(int x = 0; x<sizeX_; ++x)
		for(int y = 0; y<sizeY_; ++y)
			if (!items_[x][y]->isEmpty()) return false;

	return true;
}

void GridLayout::updateGeometry(int, int)
{
	// Get the widest and tallest items
	QVector<int> widestInColumn(sizeX_, 0);
	QVector<int> tallestInRow(sizeY_, 0);
	for(int x=0; x<sizeX_; ++x)
	{
		for(int y=0; y<sizeY_; ++y)
			if (items_[x][y] != NULL)
			{
				if (items_[x][y]->width() > widestInColumn[x]) widestInColumn[x] = items_[x][y]->width();
				if (items_[x][y]->height() > tallestInRow[y]) tallestInRow[y] = items_[x][y]->height();
			}
	}

	// Compute size
	int totalWidth = 0;
	for (int x = 0; x<sizeX_; ++x) totalWidth += widestInColumn[x];
	if (sizeX_ > 1) totalWidth += (sizeX_-1)*style()->horizontalSpace();

	int totalHeight = 0;
	for (int y = 0; y<sizeY_; ++y) totalHeight += tallestInRow[y];
	if (sizeY_ > 1) totalHeight += (sizeY_-1)*style()->verticalSpace();

	setInnerSize(totalWidth, totalHeight);

	// Set item positions

	int left = xOffset();
	for(int x=0; x<sizeX_; ++x)
	{
		int top = yOffset();
		for(int y=0; y<sizeY_; ++y)
		{
			if (items_[x][y] != NULL)
			{
				int xPos = left;
				if (style()->horizontalAlignment() == LayoutStyle::CenterAlignment) xPos += (widestInColumn[x] - items_[x][y]->width())/2;
				else if (style()->horizontalAlignment() == LayoutStyle::RightAlignment) xPos += (widestInColumn[x] - items_[x][y]->width());

				int yPos = top;
				if (style()->verticalAlignment() == LayoutStyle::CenterAlignment) yPos += (tallestInRow[y] - items_[x][y]->height())/2;
				else if (style()->verticalAlignment() == LayoutStyle::BottomAlignment) yPos += (tallestInRow[y] - items_[x][y]->height());

				items_[x][y]->setPos(xPos, yPos);
			}

			top += tallestInRow[y];
			top += style()->verticalSpace();
		}

		left += widestInColumn[x];
		left += style()->horizontalSpace();
	}
}

QPoint GridLayout::focusedElementIndex() const
{
	for(int x=0; x<sizeX_; ++x)
		for(int y=0; y<sizeY_; ++y)
			if ( items_[x][y]->childHasFocus()) return QPoint(x,y);

	return QPoint(-1,-1);
}

bool GridLayout::focusChild(FocusTarget location)
{
	if (sizeX_ == 0 || sizeY_ == 0) return false;

	Item* toFocus = NULL;
	switch (location)
	{
		case FOCUS_DEFAULT:
			toFocus = findFirstVertical(true);
			break;
		case FOCUS_TOPMOST:
			toFocus = findFirstVertical(true);
			break;
		case FOCUS_BOTTOMMOST:
			toFocus = findFirstVertical(false);
			break;
		case FOCUS_LEFTMOST:
			toFocus = findFirstHorizontal(true);
			break;
		case FOCUS_RIGHTMOST:
			toFocus = findFirstHorizontal(false);
			break;
		case FOCUS_UP:
			toFocus = findNext(0,-1);
			break;
		case FOCUS_DOWN:
			toFocus = findNext(0, 1);
			break;
		case FOCUS_LEFT:
			toFocus = findNext(-1,0);
			break;
		case FOCUS_RIGHT:
			toFocus = findNext(1, 0);
			break;
	}

	return Item::focusChild(toFocus);
}

Item* GridLayout::findFirstVertical(bool startFromTop)
{
	for(int y = 0; y<sizeY_; ++y)
		for(int x = 0; x<sizeX_; ++x)
		{
			int yIndex = startFromTop ? y : (sizeY_-y-1);
			if (items_[x][yIndex] != NULL) return items_[x][yIndex];
		}

	return NULL;
}

Item* GridLayout::findFirstHorizontal(bool startFromLeft)
{
	for(int x = 0; x<sizeX_; ++x)
		for(int y = 0; y<sizeY_; ++y)
		{
			int xIndex = startFromLeft ? x : (sizeX_-x-1);
			if (items_[xIndex][y] != NULL) return items_[xIndex][y];
		}

	return NULL;
}

Item* GridLayout::findNext(int dx, int dy)
{
	QPoint current = focusedElementIndex();
	if (current.x() < 0 || current.y() < 0) return NULL;

	int x = current.x() + dx;
	int y = current.y() + dy;

	while (x>= 0 && x < sizeX_ && y>=0 && y < sizeY_)
	{
		if (items_[x][y] != NULL) return items_[x][y];

		x+= dx;
		y+= dy;
	}

	return NULL;
}

}
