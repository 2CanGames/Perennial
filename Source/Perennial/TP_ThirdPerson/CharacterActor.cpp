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
		PlayerInventory = new UInventory();
	}

	_CurrentStage = YOUNG;

	DaysAlive = 0;
	
}

// Called every frame
void ACharacterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ACharacterActor & ACharacterActor::GetInstance()
{
	static ACharacterActor instance;
	return instance;
}
