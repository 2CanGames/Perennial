// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "PlantLookupTable.h"
#include "PlantEventListener.h"
#include <memory>
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

	bIsFertilized = false;
	bIsHarvestable = false;
	Quality = 0;
	DaysAlive = 0;
	_TimeSinceLastWatering = 0;
	_CurrentStage = EPlantStage::NO_PLANT;
}

// Called when the game starts or when spawned
void APlantActor::BeginPlay()
{
	Super::BeginPlay();
	OnDayEndedListener = new PlantEventListener(this);
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
	TArray<UActorComponent*> Icons = GetComponentsByClass(UBillboardComponent::StaticClass());
	for (int i = 0; i < Icons.Num(); i++) {
		UBillboardComponent* icon = (UBillboardComponent*)Icons[i];
		if (icon->GetName().Contains("Water")) WaterIcon = icon;
		/*else if (icon->GetName().Contains("Harvest")) WaterIcon = icon;
		else if (icon->GetName().Contains("Fertilized")) WaterIcon = icon;*/
	}
	SetIsWatered(false);
}

void APlantActor::DayEnded()
{
	//Make checks on bool flags

	//If I haven't been watered today, update TimeSinceLastWatering
	if (bIsWatered) _TimeSinceLastWatering = 0;
	else _TimeSinceLastWatering++;

	//If I'm fertilized, then speed up days I have been alive
	//TODO: have this number be an exposed variable (FertilizerEffectiveness)
	if (bIsFertilized) DaysAlive += 2;
	else DaysAlive++;

	//If TimeSinceLastWater > a number, then Die
	if (_TimeSinceLastWatering > 2) {
		Die();
		return;
	}

	//Revert isWatered state
	//Revert isFertilized state

	SetIsWatered(false);
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
	SetIsWatered(false);
	bIsFertilized = false;
	bIsHarvestable = false;
	DaysAlive = 0;
	SetStage(EPlantStage::SEED);
}

/*
	Plants the plant.
	Sets this PlantActor to the appropriate type of plant
*/
void APlantActor::Plant(UInventoryItem * item)
{
	//Do not do anything if my state is anything but NO_PLANT
	if (_CurrentStage != EPlantStage::NO_PLANT) return;

	//Init plant based on type of item
	Quality = item->getQuality();
	//TODO: Eventually get name from item
	InitPlant("tomato");
}

/*
	Waters the plant. Right now sets a bool to true
	TODO: call some UI or material function here
*/
void APlantActor::Water()
{
	SetIsWatered(true);
}

/*
	Fertilizes the plant. Right now just sets a bool to true
	TODO: call some UI function here
*/
void APlantActor::Fertilize()
{
	bIsFertilized = true;
}

/*
	Grows the plant to the next stage, resets DaysAlive, and changes the mesh
*/
void APlantActor::Grow()
{
	//Updates the stage 
	switch (_CurrentStage) {
	case EPlantStage::NO_PLANT:
		//TODO: remove this once we have planting working
		InitPlant("tomato");
		return;
	case EPlantStage::SEED:
		SetStage(EPlantStage::BUDDING);
		break;
	case EPlantStage::BUDDING:
		SetStage(EPlantStage::GROWN);
		break;
	case EPlantStage::GROWN:
		bIsHarvestable = true;
		break;
	};

	DaysAlive = 0;
}

/*
	Plant values are reset to default and we revert back to NO_PLANT state
*/
void APlantActor::Die()
{
	//Reset
	SetIsWatered(false);
	bIsFertilized = false;
	bIsHarvestable = false;
	Quality = 0;
	DaysAlive = 0;
	//Revert to plantable soil
	SetStage(EPlantStage::NO_PLANT);
}

/*
	Sets the type of plant
*/
void APlantActor::SetType(EPlantType newType)
{
	_Type = newType;
}
/*
	Sets the type of plant given a type in string format
*/
void APlantActor::SetType(FString newType)
{
	newType = newType.ToLower();

	if (newType == "tree") _Type = EPlantType::TREE;
	else if (newType == "vine") _Type = EPlantType::VINE;
	else if (newType == "root") _Type = EPlantType::ROOT;
}

void APlantActor::SetIsWatered(bool newBool)
{
	bIsWatered = newBool;
	if (WaterIcon == NULL || WaterIcon == nullptr) return;
	//If this plant is watered or is not planted, then don't show the icon
	if (bIsWatered || (_CurrentStage == EPlantStage::NO_PLANT)) {
		WaterIcon->SetVisibility(false);
	}
	else {
		WaterIcon->SetVisibility(true);
	}
}

void APlantActor::SetIsHarvestable(bool newBool)
{
}

void APlantActor::SetIsFertilized(bool newBool)
{
}

/*
	Returns type of plant
*/
EPlantType APlantActor::GetType() const
{
	return _Type;
}

/*
	Changes the stage at which the plant is at and swaps for the appropriate
	mesh
*/
void APlantActor::SetStage(EPlantStage newStage)
{
	_CurrentStage = newStage;
	USkeletalMesh** newMesh = (MeshMap.Find(_CurrentStage));
	if (newMesh) {
		USkeletalMeshComponent* Root = (USkeletalMeshComponent*) GetRootComponent();
		Root->SetSkeletalMesh(*newMesh, false);
		if(&(*newMesh)->Materials[1] && (*newMesh)->Materials[1].MaterialInterface) 
			Root->SetMaterial(0, (*newMesh)->Materials[1].MaterialInterface);
	}
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
	SetStage(EPlantStage::NO_PLANT);

	//return TArray<InventoryItem>();
}