#include "Headers/MinMaxResult.h"

MinMaxResult::MinMaxResult()
{
	Value = 0;
}

MinMaxResult::MinMaxResult(int value, int depth)
{
	Value = value;
	Depth = depth;
}

MinMaxResult::~MinMaxResult()
{

}