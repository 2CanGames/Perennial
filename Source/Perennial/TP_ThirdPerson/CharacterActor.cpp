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
	OnDayEndedListener = new CharacterEventListener(this);
}

void ACharacterActor::EndPlay(EEndPlayReason::Type Reason)
{
	delete OnDayEndedListener;
}

// Called every frame
void ACharacterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ACharacterActor* & ACharacterActor::GetInstance()
{
	static ACharacterActor* instance;
	return instance;
}

void ACharacterActor::DayEnded()
{
	DaysAlive++;
	NumFertilizers = 3;
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
		return true;
	}
}
