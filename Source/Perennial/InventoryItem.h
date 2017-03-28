// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlantLookupTable.h"

/**
 * 
 */
class PERENNIAL_API InventoryItem
{
private:
	UDataTable* PlantLookupTable;
	FString plantName;

public:
	InventoryItem(FString plantName);
	~InventoryItem();

	int getQuality();
};
