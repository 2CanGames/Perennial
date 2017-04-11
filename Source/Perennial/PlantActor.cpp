 // Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "PlantLookupTable.h"
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

	PlantMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlantMesh"));
	WaterIcon = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("WaterIcon"));
	FertilizerEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FertilizerEffect"));

	RootComponent = PlantMesh;

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
	SetStage(EPlantStage::NO_PLANT);
	SetIsWatered(false);
	SetIsFertilized(false);
}

void APlantActor::EndPlay(EEndPlayReason::Type Reason)
{
	
}

void APlantActor::processEvent()
{
	DayEnded();
}


void APlantActor::DayEnded()
{
	//if (_CurrentStage == EPlantStage::NO_PLANT) return;
	//Make checks on bool flags
	
	//If I haven't been watered today, update TimeSinceLastWatering
	if (bIsWatered) _TimeSinceLastWatering = 0;
	else _TimeSinceLastWatering++;

	//If I'm fertilized, then speed up days I have been alive
	if (bIsFertilized) DaysAlive += FertilizerSpeed;
	else DaysAlive++;

	//If TimeSinceLastWater > a number, then Die
	if (_TimeSinceLastWatering > 2) {
		Die();
		return;
	}

	//Determine if we need to grow based on how many days we have been alive
	//if yes, call grow
	if (DaysAlive >= DaysToGrow) {
		int GrowIterations = DaysAlive / DaysToGrow;
		//If fertilizer makes plant grow 2 stages in one day, then grow x amt of iterations
		for (int i = 0; i < GrowIterations; i++)
			Grow();
	}

	//Revert isWatered state
	//Revert isFertilized state

	SetIsWatered(false);
	SetIsFertilized(false);
}

// Called every frame
void APlantActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlantActor::InitPlant(FString name)
{
	if (_CurrentStage != EPlantStage::NO_PLANT) return;

	if (name.IsEmpty())
		name = "apple";

	static const FString ContextString(TEXT("GENERAL"));

	FPlantLookupTable* PLookupRow = PlantLookupTable->FindRow<FPlantLookupTable>(
		*name,
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

	//Dynamically load in harvest mesh
	HarvestMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), NULL, *PLookupRow->Plant_Model));
	Quality = PLookupRow->Quality;
	//DaysToGrow = PLookupRow->Days_To_Next_Stage;
	SetType(PLookupRow->Plant_Type);
	PlantName = name.ToLower();
	//Set default parameters
	SetStage(EPlantStage::SEED);
	SetIsWatered(false);
	SetIsFertilized(false);
	SetIsHarvestable(false);		
	DaysAlive = 0;
}

/*
	Plants the plant.
	Sets this PlantActor to the appropriate type of plant
*/
void APlantActor::Plant(UInventoryItem * item)
{
	//Do not do anything if my state is anything but NO_PLANT
	if (_CurrentStage != EPlantStage::NO_PLANT) return;
	if (!item) {
		item = CreateDefaultSubobject<UInventoryItem>("item");
		
	}
	//Init plant based on type of item
	Quality = item->getQuality();
	InitPlant(item->getPlantName());
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
	SetIsFertilized(true);
}

/*
	Grows the plant to the next stage, resets DaysAlive, and changes the mesh
*/
void APlantActor::Grow()
{
	//Updates the stage 
	switch (_CurrentStage) {
	case EPlantStage::NO_PLANT:
		//TODO: Get rid of initplant. 
		InitPlant(PlantName);
		return;
	case EPlantStage::SEED:
		SetStage(EPlantStage::BUDDING);
		break;
	case EPlantStage::BUDDING:
		SetStage(EPlantStage::GROWN);
		break;
	case EPlantStage::GROWN:
		SetIsHarvestable(true);
		break;
	};

	DaysAlive = 0;
}

/*
	Plant values are reset to default and we revert back to NO_PLANT state
*/
void APlantActor::Die()
{
	//Revert to plantable soil
	SetStage(EPlantStage::NO_PLANT);
	//Reset
	SetIsWatered(false);
	SetIsFertilized(false);
	SetIsHarvestable(false);
	Quality = 0;
	DaysAlive = 0;
}

/*
	Sets the type of plant
*/
void APlantActor::SetType(EPlantType newType)
{
	_Type = newType;
	MeshMap.Emplace(EPlantStage::GROWN, *(GrownMeshMap.Find(_Type)));
}
/*
	Sets the type of plant given a type in string format
*/
void APlantActor::SetType(FString newType)
{
	newType = newType.ToLower();

	if (newType == "tree") SetType(EPlantType::TREE);
	else if (newType == "vine") SetType(EPlantType::VINE);
	else if (newType == "root") SetType(EPlantType::ROOT);
	else if (newType == "bush") SetType(EPlantType::BUSH);
}

void APlantActor::SetIsWatered(bool newBool)
{
	bIsWatered = newBool;
	if (!WaterIcon) return;
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
	bIsHarvestable = newBool;
	TArray<FName> Sockets = PlantMesh->GetAllSocketNames();
	if (bIsHarvestable) {
		
		for (auto Socket : Sockets) {
			if (!Socket.ToString().Contains("Fruit")) continue;
			const USkeletalMeshSocket* s = PlantMesh->GetSocketByName(Socket);
			AHarvestable * harvestable = GetWorld()->SpawnActor<AHarvestable>(AHarvestable::StaticClass());
			Harvestables.Add(harvestable);
			harvestable->SetHarvestableMesh(HarvestMesh);
			harvestable->AttachToComponent(PlantMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, Socket);
		}
	}
	else {
		for (auto Harvest : Harvestables) {
			Harvest->RemoveFromRoot();
			Harvest->Destroy();
		}
	}
	
}

void APlantActor::SetIsFertilized(bool newBool)
{
	bIsFertilized = newBool;
	if (!FertilizerEffect) return;
	if (bIsFertilized && !(_CurrentStage == EPlantStage::NO_PLANT)) {
		FertilizerEffect->ActivateSystem();
	}
	else {
		FertilizerEffect->DeactivateSystem();
	}
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
	//We want to get the reference to its grown mesh
	USkeletalMesh** newMesh = (MeshMap.Find(_CurrentStage));
	if (newMesh) {
		PlantMesh->SetSkeletalMesh(*newMesh, false);
		if((*newMesh)->Materials.Num() > 1 && &(*newMesh)->Materials[1] && (*newMesh)->Materials[1].MaterialInterface) 
			PlantMesh->SetMaterial(0, (*newMesh)->Materials[1].MaterialInterface);
	}
}

EPlantStage APlantActor::GetStage() const
{
	return _CurrentStage;
}

TArray<UInventoryItem *> APlantActor::Harvest()
{	
	TArray<UInventoryItem *> HarvestResult;
	if (!bIsHarvestable) return HarvestResult;
	//Get some fruit and seeds
	for (int i = 0; i < 3; i++) {
		UInventoryItem* harvest = ConstructObject<UInventoryItem>(UInventoryItem::StaticClass(), (UObject*)GetTransientPackage(), *PlantName);
		if (harvest != NULL)
		{
			HarvestResult.Add(harvest);
		}
		else
		{
			// Failed to create object
		}
		
	}

	//Delete this object
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(
			1,
			2.0f,
			FColor::Blue,
			TEXT("Harvesting")
		);
	}
	
	SetIsHarvestable(false);

	return HarvestResult;
}