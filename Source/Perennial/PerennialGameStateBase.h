// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EventListener.h"

#include "GameFramework/GameStateBase.h"
#include "PerennialGameStateBase.generated.h"

// Season Enumeration
UENUM(BlueprintType)
enum class Season : uint8
{
	SPRING	UMETA(DisplayName = "SPRING"),
	SUMMER	UMETA(DisplayName = "SUMMER"),
	FALL	UMETA(DisplayName = "FALL"),
	WINTER	UMETA(DisplayName = "WINTER")
};

/**
 * 
 */
UCLASS()
class PERENNIAL_API APerennialGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
private:
	const int DAYS_PER_SEASON = 5;
	const int NUM_OF_SEASONS = 4;
	int currentAmountOfDays = 0;
	Season currentSeason = Season::SPRING;

	TArray<AEventListener*> listeners;
public:

	// advances to the next day and broadcasts an event to all listenters
	UFUNCTION(BlueprintCallable)
	void advanceToNextDay();

	void broadcast();

	// signs up a EventListener to the TimeController to receive broadcasts
	void eventListenerSignUp(AEventListener *listener);

	void eventListenerRemove(AEventListener *listener);

	int getCurrentAmountOfDays();
};
