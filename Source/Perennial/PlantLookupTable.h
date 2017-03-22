// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
/**
 * 
 */
USTRUCT(Blueprintable)
struct PERENNIAL_API FPlantLookupTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Plant")
		FString Plant_Name;

	UPROPERTY(BlueprintReadOnly, Category = "Plant")
		FString Plant_Type;

	UPROPERTY(BlueprintReadOnly, Category = "Plant Assets")
		FString Plant_Model;

	UPROPERTY(BlueprintReadOnly, Category = "Plant Assets")
		FString Plant_Portrait;


};
