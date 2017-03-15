#include "Headers/ColorOperations.h"

EColor inverseColor(EColor color)
{
	if (color == EColor::White)
		return EColor::Black;
	else
		return EColor::White;
}