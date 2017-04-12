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
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Inventory Items")
	TArray<AInventoryItem*> items;

public:
	UInventory();
	~UInventory();

	void addItemToInventory(AInventoryItem *item);

	void removeItemToInventory(AInventoryItem *item);

	void removeItemToInventory(FString plantName);

	int getTotalQualityPoints();
	
	
	
};
