// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "PlantActor.h"


// Sets default values
APlantActor::APlantActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlantActor::BeginPlay()
{
	Super::BeginPlay();
	//Find the lookup table and have a pointer to it
	static ConstructionHelpers::FObjectFinder<UDataTable>
		PlantLookupDataTable_BP(TEXT("DataTable'/Game/Data"));
	PlantLookupTable = PlantLookupTable_BP.Object;
	
}

// Called every frame
void APlantActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlantActor::InitPlant(FString name, PlantType type)
{
	_CurrentType = type;
	static const FString ContextString(TEXT("GENERAL"));
}

void APlantActor::SetStage(PlantStage newStage)
{
	_CurrentStage = newStage;
}

PlantStage APlantActor::GetStage()
{
	return _CurrentStage;
}

void APlantActor::Harvest()
{
}

