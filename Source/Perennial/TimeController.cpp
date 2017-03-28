// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "TimeController.h"

ATimeController* ATimeController::instance = NULL;

ATimeController::ATimeController() {

}

ATimeController* ATimeController::getInstance() {
	if (instance == NULL) {
		instance = new ATimeController();
	}
	
	return instance;
}

void ATimeController::advanceToNextDay() {
	currentAmountOfDays++;

	/*if (currentAmountOfDays % DAYS_PER_SEASON == 0) {

		int numberOfSeasonsPassed = currentAmountOfDays / DAYS_PER_SEASON;
		int seasonNumber = numberOfSeasonsPassed % NUM_OF_SEASONS;
		
		switch (seasonNumber) {
		case 0:
			currentSeason = Season::SPRING;
			break;
		case 1:
			currentSeason = Season::SUMMER;
			break;
		case 2:
			currentSeason = Season::FALL;
			break;
		case 3:
			currentSeason = Season::WINTER;
			break;
		}
	}

	broadcast();*/
}

void ATimeController::broadcast() {

	std::vector<EventListener*>::iterator itr;
	for (itr = listeners.begin(); itr < listeners.end(); ++itr)
	{
		(*itr)->processEvent();
	}
}

void ATimeController::eventListenerSignUp(EventListener *listener) {
	listeners.push_back(listener);
}

int ATimeController::getCurrentAmountOfDays() {
	return currentAmountOfDays;
}


