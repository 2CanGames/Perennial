// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include <vector>

#include "InventoryItem.h"

#include "UObject/NoExportTypes.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class PERENNIAL_API UInventory : public UObject
{
	GENERATED_BODY()
private:
	std::vector<UInventoryItem*> items;

public:
	UInventory();
	~UInventory();

	void addItemToInventory(UInventoryItem *item);

	void removeItemToInventory(UInventoryItem *item);

	int getTotalQualityPoints();
	
	
	
};
