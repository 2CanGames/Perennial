// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PlantLookupTable.h"
#include "PlantDictionary.generated.h"

UENUM(BlueprintType)
enum EPlantType {
	TREE,
	ROOT,
	VINE,
	BUSH
};

UENUM(BlueprintType)
enum EPlantStage {
	NO_PLANT,
	SEED,
	BUDDING,
	GROWN
};

USTRUCT()
struct PERENNIAL_API FPlant {

	GENERATED_USTRUCT_BODY()

	FString Name;
	EPlantType Type;
	int Quality;
};


UCLASS()
class PERENNIAL_API APlantDictionary : public AActor
{
	GENERATED_BODY()
	
private:
	//Data lookup table for plant information
	UDataTable* PlantLookupTable;

	TMap<EPlantType, TArray<FPlant>> PlantDictionary;

	EPlantType MapToPlantType(FString type) const;

public:	
	// Sets default values for this actor's properties
	APlantDictionary();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString GetRandomPlantOfType(EPlantType type);
	
};