#pragma once

class MaterialRateData
{
public:
	MaterialRateData();
	~MaterialRateData();

	int WhiteMaterialRate;
	int BlackMaterialRate;
	int BoardMaterialRate;

	int WhiteMaterialRateWithoutKing;
	int BlackMaterialRateWithoutKing;
};