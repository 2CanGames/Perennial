// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "InventoryItem.h"


// Sets default values
AInventoryItem::AInventoryItem()
{
	if (!PlantLookupTable) 
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> PlantLookupDataTable_BP(TEXT("DataTable'/Game/Data/PlantData.PlantData'"));

		if (PlantLookupDataTable_BP.Object) 
		{
			PlantLookupTable = PlantLookupDataTable_BP.Object;
		}
	}

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	plantName = "DEFAULT_PLANT_NAME";
	isSeed = false;
}

// Called when the game starts or when spawned
void AInventoryItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInventoryItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInventoryItem::setPlantName(FString PlantName)
{
	this->plantName = PlantName;
}

int AInventoryItem::getQuality() 
{
	static const FString ContextString(TEXT("GENERAL"));

	FPlantLookupTable* PLookupRow = PlantLookupTable->FindRow<FPlantLookupTable>(
		*plantName,
		ContextString
		);

	if (PLookupRow) 
	{
		return PLookupRow->Quality;
	}

	return 0;
}

FString AInventoryItem::getPlantName() 
{
	return plantName;
}

bool AInventoryItem::getIsSeed() 
{
	return isSeed;
}

void AInventoryItem::setIsSeed(bool IsSeed) 
{
	this->isSeed = IsSeed;
}

