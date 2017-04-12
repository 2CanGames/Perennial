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

	UInventoryItem* Seed1 = ConstructObject<UInventoryItem>(UInventoryItem::StaticClass(), (UObject*)GetTransientPackage(), *(FString("tomato")));
	UInventoryItem* Seed2 = ConstructObject<UInventoryItem>(UInventoryItem::StaticClass(), (UObject*)GetTransientPackage(), *(FString("strawberry")));
	UInventoryItem* Seed3 = ConstructObject<UInventoryItem>(UInventoryItem::StaticClass(), (UObject*)GetTransientPackage(), *(FString("apple")));

	Seed1->isSeed = true;
	Seed2->isSeed = true;
	Seed3->isSeed = true;

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

void ACharacterActor::PlantSeed(APlantActor* CurrentPlant, UInventoryItem* Seed)
{
	// Plant the seed
	CurrentPlant->Plant(Seed);

	// Delete seeds from player inventory
	PlayerInventory->removeItemToInventory(Seed);
}

void ACharacterActor::Harvest(APlantActor* CurrentPlant)
{
	TArray<UInventoryItem *> ItemsForInventory = CurrentPlant->Harvest();

	// Add all items in array to player's inventory
	for (auto& Item : ItemsForInventory) 
	{
		PlayerInventory->addItemToInventory(Item);
	}
}
