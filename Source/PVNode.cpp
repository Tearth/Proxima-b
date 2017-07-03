#include "Headers/PVNode.h"

PVNode::PVNode() : PVNode(0, Move())
{

}

PVNode::PVNode(unsigned long long int hash, Move move)
{ 
	BoardHash = hash;
	BestMove = move;
}

PVNode::~PVNode()
{

}