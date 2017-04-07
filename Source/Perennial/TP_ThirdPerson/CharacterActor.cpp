// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "CharacterActor.h"


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
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			GEngine->ScreenMessages.Num() + 1,
			6.0f,
			FColor::Green,
			"ACharacterActor processing event"
		);
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
