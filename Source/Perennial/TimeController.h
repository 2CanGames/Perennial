// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include <vector>

#include "EventListener.h"
#include "GameFramework/GameStateBase.h"
#include "TimeController.generated.h"

// Season Enumeration
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class Season : uint8
{
	SPRING 	UMETA(DisplayName = "Spring"),
	SUMMER 	UMETA(DisplayName = "Summer"),
	FALL	UMETA(DisplayName = "Fall"),
	WINTER	UMETA(DisplayName = "Winter")
};

/**
 * 
 */
UCLASS()
class PERENNIAL_API ATimeController : public AGameStateBase
{
	GENERATED_BODY()
	
// private variables
private:
	
	const int DAYS_PER_SEASON = 5;
	const int NUM_OF_SEASONS = 4;
	int currentAmountOfDays = 0;
	Season currentSeason = Season::SPRING;
	std::vector<EventListener*> listeners;

	static ATimeController *instance;

// private methods
private:

	// Constructor
	ATimeController();

// public methods
public:

	// returns an instance of the TimerController
	static ATimeController* getInstance();

	// advances to the next day and broadcasts an event to all listenters
	void advanceToNextDay();

	// broadcasts a new day event to all listeners
	void broadcast();

	// signs up a EventListener to the TimeController to receive broadcasts
	void eventListenerSignUp(EventListener *listener);
};
