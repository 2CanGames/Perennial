// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "CharacterActor.h"
#include "PlantActor.h"
#include "InventoryItem.h"


// Sets default values
ACharacterActor::ACharacterActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterActor::BeginPlay()
{
	Super::BeginPlay();

	if (!PlayerInventory)
	{
		PlayerInventory = NewObject<UInventory>(UInventory::StaticClass());
	}

	_CurrentStage = YOUNG;
	DaysAlive = 0;
	NumFertilizers = 3;
	TotalQualityPoints = 0;

	AInventoryItem* Seed1 = GetWorld()->SpawnActor<AInventoryItem>();
	AInventoryItem* Seed2 = GetWorld()->SpawnActor<AInventoryItem>();
	AInventoryItem* Seed3 = GetWorld()->SpawnActor<AInventoryItem>();

	Seed1->setIsSeed(true);
	Seed2->setIsSeed(true);
	Seed3->setIsSeed(true);

	Seed1->setPlantName("tomato");
	Seed2->setPlantName("lemon");
	Seed3->setPlantName("strawberry");

	PlayerInventory->addItemToInventory(Seed1);
	PlayerInventory->addItemToInventory(Seed2);
	PlayerInventory->addItemToInventory(Seed3);
	
}

void ACharacterActor::EndPlay(EEndPlayReason::Type Reason)
{

}

// Called every frame
void ACharacterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterActor::processEvent()
{
	DayEnded();
}

void ACharacterActor::DayEnded()
{
	DaysAlive++;
	NumFertilizers = 3;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Fertilizers: ") + FString::FromInt(NumFertilizers));
	}
}

bool ACharacterActor::DeleteFertilizer()
{
	if (NumFertilizers <= 0)
	{
		return false;
	}
	else
	{
		NumFertilizers--;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Fertilizers: ") + FString::FromInt(NumFertilizers));
		}

		return true;
	}
}

void ACharacterActor::PlantSeed(APlantActor* CurrentPlant, AInventoryItem* Seed)
{
	// Plant the seed
	CurrentPlant->Plant(Seed);

	// Delete seed from player inventory
	PlayerInventory->removeItemToInventory(Seed);
}

void ACharacterActor::Harvest(APlantActor* CurrentPlant)
{
	TArray<AInventoryItem *> ItemsForInventory = CurrentPlant->Harvest();

	// Add all items in array to player's inventory
	for (auto& Item : ItemsForInventory) 
	{
		PlayerInventory->addItemToInventory(Item);
	}

	TotalQualityPoints = PlayerInventory->getTotalQualityPoints();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Total Quality Points: ") + FString::FromInt(TotalQualityPoints));
	}
}

int ACharacterActor::GetTotalQualityPoints()
{
	return PlayerInventory->getTotalQualityPoints();
}
