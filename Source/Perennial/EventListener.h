// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EventListener.h"
#include "EventListener.generated.h"

/**
 * An inteface that allows objects to listen to the 
 * TimeController and implement unique behavior
 */
UCLASS()
class PERENNIAL_API AEventListener : public AActor
{
	GENERATED_BODY()

public:

	EventListener();
	~EventListener();

	// process event with derived class behavior
	virtual void processEvent() = 0;
};
