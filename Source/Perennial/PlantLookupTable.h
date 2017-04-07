// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/DataTable.h"
#include "PlantLookupTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PERENNIAL_API FPlantLookupTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Data")
		FString Plant_Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Data")
		int Quality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Data")
		int Days_To_Next_Stage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "Plant Assets")
		FString Plant_Model;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Assets")
		FString Plant_Portrait;
};