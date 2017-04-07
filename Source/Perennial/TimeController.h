// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include <vector>

#include "EventListener.h"

// Season Enumeration
enum class Season
{
	SPRING,
	SUMMER,
	FALL,
	WINTER
};

/**
 * 
 */
class PERENNIAL_API ATimeController
{
private:
	
	const int DAYS_PER_SEASON = 5;
	const int NUM_OF_SEASONS = 4;
	int currentAmountOfDays = 0;
	Season currentSeason = Season::SPRING;
	std::vector<AEventListener*> listeners;

	static ATimeController *instance;

	// Constructor
	ATimeController();

	// broadcasts a new day event to all listeners
	void broadcast();

public:

	// returns an instance of the TimerController
	static ATimeController* getInstance();

	// advances to the next day and broadcasts an event to all listenters
	void advanceToNextDay();

	// signs up a EventListener to the TimeController to receive broadcasts
	void eventListenerSignUp(AEventListener *listener);

	void eventListenerRemove(AEventListener *listener);

	int getCurrentAmountOfDays();
};
