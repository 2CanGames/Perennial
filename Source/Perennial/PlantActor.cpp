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
	
	//https://wiki.unrealengine.com/Using_excel_to_store_gameplay_data_-_DataTables
	//Find the lookup table and have a pointer to it
	if (!PlantLookupTable) {
		static ConstructorHelpers::FObjectFinder<UDataTable> PlantLookupDataTable_BP(TEXT("DataTable'/Game/Data/PlantData.PlantData'"));

		if (PlantLookupDataTable_BP.Object) {
			PlantLookupTable = PlantLookupDataTable_BP.Object;
		}
	} 

	bIsWatered = false;
	bIsFertilized = false;
	bIsHarvestable = false;
	Quality = 0;
	DaysAlive = 0;
	_CurrentStage = EPlantStage::NO_PLANT;
}

// Called when the game starts or when spawned
void APlantActor::BeginPlay()
{
	Super::BeginPlay();
	static const FString ContextString(TEXT("GENERAL"));

	FPlantLookupTable* PLookupRow = PlantLookupTable->FindRow<FPlantLookupTable>(
		*PlantName,
		ContextString
		);

	if (PLookupRow) {
		SetType(PLookupRow->Plant_Type);
		Quality = PLookupRow->Quality;
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(
				GEngine->ScreenMessages.Num() + 1,
				6.0f,
				FColor::Green,
				*PLookupRow->Plant_Type
			);
		}
	}
}

void APlantActor::DayEnded()
{
	//Make checks on bool flags

	//If I haven't been watered today, update TimeSinceLastWatering
	//if (bIsWatered) _TimeSinceLastWatering = 0;
	//else _TimeSinceLastWatering++;

	////If I'm fertilized, then speed up days I have been alive
	////TODO: have this number be an exposed variable (FertilizerEffectiveness)
	//if (bIsFertilized) DaysAlive += 2;
	//else DaysAlive++;

	////If TimeSinceLastWater > a number, then Die
	//if (_TimeSinceLastWatering > 2) {
	//	Die();
	//	return;
	//}

	//Revert isWatered state
	//Revert isFertilized state

	bIsWatered = false;
	bIsFertilized = false;

	//Determine if we need to grow based on how many days we have been alive
	//if yes, call grow
	//TODO: Change 3 to DaysToGrow variable
	if (DaysAlive >= 0) {
		Grow();
	}
}

// Called every frame
void APlantActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlantActor::InitPlant(FString name)
{
	if (_CurrentStage != EPlantStage::NO_PLANT) return;

	PlantName = name.ToLower();
	//Set default parameters
	bIsWatered = false;
	bIsFertilized = false;
	bIsHarvestable = false;
	Quality = 0;
	DaysAlive = 0;
	_CurrentStage = EPlantStage::SEED;
	USkeletalMesh** newMesh = (MeshMap.Find(_CurrentStage));
	
	if(newMesh)
		((USkeletalMeshComponent*)GetRootComponent())->SetSkeletalMesh(*newMesh);	
}

void APlantActor::Plant(InventoryItem * item)
{
	
}

void APlantActor::Water()
{
	bIsWatered = true;
}

void APlantActor::Fertilize()
{
	bIsFertilized = true;
}

/*
Grows the plant to the next stage
*/
void APlantActor::Grow()
{
	//Updates the stage 
	switch (_CurrentStage) {
	case EPlantStage::NO_PLANT:
		InitPlant("tomato");
		return;
	case EPlantStage::SEED:
		_CurrentStage = EPlantStage::BUDDING;
		break;
	case EPlantStage::BUDDING:
		_CurrentStage = EPlantStage::GROWN;
		break;
	case EPlantStage::GROWN:
		bIsHarvestable = true;
		break;
	};
	USkeletalMesh** newMesh = (MeshMap.Find(_CurrentStage));
	
	if (newMesh)
		((USkeletalMeshComponent*)GetRootComponent())->SetSkeletalMesh(*newMesh);
	else {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(
				1,
				2.0f,
				FColor::Blue,
				TEXT("Mesh cannot be swapped")
			);
		}
	}
	//((USkeletalMeshComponent*)GetRootComponent())->SetSkeletalMesh(*(MeshMap.Find(_CurrentStage)));
	DaysAlive = 0;

	if (bIsHarvestable) Harvest();
	//Reset DaysAlive
}

void APlantActor::Die()
{
	//Revert to plantable soil
}

void APlantActor::SetType(EPlantType newType)
{
	_Type = newType;
}

void APlantActor::SetType(FString newType)
{
	newType = newType.ToLower();

	if (newType == "tree") _Type = EPlantType::TREE;
	else if (newType == "vine") _Type = EPlantType::VINE;
	else if (newType == "root") _Type = EPlantType::ROOT;
}


EPlantType APlantActor::GetType() const
{
	return _Type;
}

void APlantActor::SetStage(EPlantStage newStage)
{
	_CurrentStage = newStage;
}

EPlantStage APlantActor::GetStage() const
{
	return _CurrentStage;
}

void APlantActor::Harvest()
{	
	//Get some fruit and seeds


	//Delete this object
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(
			1,
			2.0f,
			FColor::Blue,
			TEXT("Harvesting")
		);
	}
	_CurrentStage = EPlantStage::NO_PLANT;
	((USkeletalMeshComponent*)GetRootComponent())->SetSkeletalMesh(*(MeshMap.Find(_CurrentStage)));
}
