// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlantLookupTable.h"

#include "UObject/NoExportTypes.h"
#include "InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class PERENNIAL_API UInventoryItem : public UObject
{
	GENERATED_BODY()
	
protected:
	UDataTable* PlantLookupTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Name")
	FString plantName;

public:
	UInventoryItem();
	UInventoryItem(FString plantName);
	~UInventoryItem();

	int getQuality();
	FString getPlantName();

	bool isSeed;
};
