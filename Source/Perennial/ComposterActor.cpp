// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "ComposterActor.h"


// Sets default values
AComposterActor::AComposterActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AComposterActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AComposterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

