// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "InventoryItem.h"

UInventoryItem::UInventoryItem() {
	if (GetName().IsEmpty())
		plantName = "DEFAULT_PLANT_NAME";
	else
		plantName = GetName();

	this->isSeed = false;
}

UInventoryItem::UInventoryItem(FString plantName)
{
	if (!PlantLookupTable) {
		static ConstructorHelpers::FObjectFinder<UDataTable> PlantLookupDataTable_BP(TEXT("DataTable'/Game/Data/PlantData.PlantData'"));

		if (PlantLookupDataTable_BP.Object) {
			PlantLookupTable = PlantLookupDataTable_BP.Object;
		}
	}
	this->plantName = plantName;

	this->isSeed = false;
}

UInventoryItem::~UInventoryItem()
{
}

int UInventoryItem::getQuality() {
	static const FString ContextString(TEXT("GENERAL"));

	FPlantLookupTable* PLookupRow = PlantLookupTable->FindRow<FPlantLookupTable>(
		*plantName,
		ContextString
		);

	if (PLookupRow) {
		//SetType(PLookupRow->Plant_Type);
		return PLookupRow->Quality;
		/*if (GEngine) {
		GEngine->AddOnScreenDebugMessage(
		GEngine->ScreenMessages.Num() + 1,
		6.0f,
		FColor::Green,
		*PLookupRow->Plant_Type
		);
		}*/
	}

	return 0;
}

FString UInventoryItem::getPlantName() {
	return plantName;
}



