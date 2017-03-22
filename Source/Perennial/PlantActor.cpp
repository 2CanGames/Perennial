// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "PlantLookupTable.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "PlantActor.h"


// Sets default values
APlantActor::APlantActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APlantActor::BeginPlay()
{
	Super::BeginPlay();
	_CurrentStage = PlantStage::SEED;
}

void APlantActor::DayEnded_Implementation()
{
}

// Called every frame
void APlantActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlantActor::InitPlant(FString name)
{
	PlantName = name.ToLower();
	//https://wiki.unrealengine.com/Using_excel_to_store_gameplay_data_-_DataTables
	//Find the lookup table and have a pointer to it
	static ConstructorHelpers::FObjectFinder<UDataTable> PlantLookupDataTable_BP(TEXT("DataTable'/Game/Data/PlantData.PlantData'"));
	PlantLookupTable = PlantLookupDataTable_BP.Object;
	static const FString ContextString(TEXT("GENERAL"));

	FPlantLookupTable* PLookupRow = PlantLookupTable->FindRow<FPlantLookupTable>(
		*PlantName,
		ContextString
		);

	if (PLookupRow) {
		SetType(PLookupRow->Plant_Type);
		Quality = PLookupRow->Quality;
		GEngine->AddOnScreenDebugMessage(
			GEngine->ScreenMessages.Num() + 1,
			6.0f,
			FColor::Green,
			*PLookupRow->Plant_Type
		);
	}
}

void APlantActor::Grow()
{
}

void APlantActor::SetType(PlantType newType)
{
	_Type = newType;
}

void APlantActor::SetType(FString newType)
{
	newType = newType.ToLower();
	if(newType == "tree") _Type = PlantType::TREE;
	else if (newType == "vine") _Type = PlantType::VINE;
	else if (newType == "root") _Type = PlantType::ROOT;
}


PlantType APlantActor::GetType() const
{
	return _Type;
}

void APlantActor::SetStage(PlantStage newStage)
{
	_CurrentStage = newStage;
}

PlantStage APlantActor::GetStage() const
{
	return _CurrentStage;
}

void APlantActor::Harvest()
{
}

