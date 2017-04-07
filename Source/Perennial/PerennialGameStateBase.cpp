// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "PerennialGameStateBase.h"


void APerennialGameStateBase::advanceToNextDay() {
	days++;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			GEngine->ScreenMessages.Num() + 1,
			6.0f,
			FColor::Green,
			"Current Day: " + FString::FromInt(days)
		);
}

// signs up a EventListener to the TimeController to receive broadcasts
void APerennialGameStateBase::eventListenerSignUp(EventListener *listener) {
	listeners.Add(listener);
}

void APerennialGameStateBase::eventListenerRemove(EventListener *listener) {
	listeners.Remove(listener);
}

int APerennialGameStateBase::getCurrentAmountOfDays() {
	return days;
}