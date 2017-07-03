#include "Headers/TableItem.h"

TableItem::TableItem() : TableItem(ValueType::None, 0, 0, Move())
{

}

TableItem::TableItem(ValueType type, int value, int depth, Move bestMove)
{
	Type = type;
	Value = value;
	Depth = depth;
	BestMove = bestMove;
}

TableItem::~TableItem()
{

}