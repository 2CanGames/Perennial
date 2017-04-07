// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Harvestable.generated.h"

UCLASS()
class PERENNIAL_API AHarvestable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHarvestable();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMeshComponent* Mesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetHarvestableMesh(USkeletalMesh * newMesh);
	USkeletalMeshComponent* GetHarvestableMesh() const;
	
};
