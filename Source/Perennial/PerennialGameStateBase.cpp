// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "PerennialGameStateBase.h"


void APerennialGameStateBase::advanceToNextDay() {
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
	}*/

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			GEngine->ScreenMessages.Num() + 1,
			6.0f,
			FColor::Green,
			"Current Day: " + FString::FromInt(currentAmountOfDays)
		);
	broadcast();
}

void APerennialGameStateBase::broadcast() {
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			GEngine->ScreenMessages.Num() + 1,
			6.0f,
			FColor::Green,
			"Broadcasting to " + FString::FromInt(listeners.Num()) + " listeners"
		);

	for(int i = 0; i < listeners.Num(); i++)
	{
		listeners[i]->processEvent();
	}
}

// signs up a EventListener to the TimeController to receive broadcasts
void APerennialGameStateBase::eventListenerSignUp(AEventListener *listener) {
	listeners.Add(listener);
}

void APerennialGameStateBase::eventListenerRemove(AEventListener *listener) {
	listeners.Remove(listener);
}

int APerennialGameStateBase::getCurrentAmountOfDays() {
	return currentAmountOfDays;
}