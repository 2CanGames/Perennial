// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "Harvestable.h"


// Sets default values
AHarvestable::AHarvestable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AHarvestable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHarvestable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHarvestable::SetHarvestableMesh(USkeletalMesh * newMesh)
{
	Mesh->SetSkeletalMesh(newMesh);
}

USkeletalMeshComponent * AHarvestable::GetHarvestableMesh() const
{
	return Mesh;
}

