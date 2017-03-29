// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "TimeController.h"
#include "PauseMenuUserWidget.h"


void UPauseMenuUserWidget::advanceTimeController() {
	
	ATimeController::getInstance()->advanceToNextDay();
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(
			GEngine->ScreenMessages.Num() + 1,
			6.0f,
			FColor::Green,
			"Working"
		);
	}
}

