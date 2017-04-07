// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EventListener.h"

#include "GameFramework/GameStateBase.h"
#include "PerennialGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class PERENNIAL_API APerennialGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
private:
	int days = 0;

	TArray<EventListener*> listeners;
public:

	// advances to the next day and broadcasts an event to all listenters
	UFUNCTION(BlueprintCallable)
	void advanceToNextDay();

	// signs up a EventListener to the TimeController to receive broadcasts
	void eventListenerSignUp(EventListener *listener);

	void eventListenerRemove(EventListener *listener);

	int getCurrentAmountOfDays();
};
