// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "PauseMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PERENNIAL_API UPauseMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void advanceTimeController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My New User Widget")
	FString MyNewWidgetName;
	
};
