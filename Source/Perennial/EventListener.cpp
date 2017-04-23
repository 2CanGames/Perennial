// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "EventListener.h"
#include "PerennialGameStateBase.h"


AEventListener::AEventListener() {

}

AEventListener::~AEventListener() {
}

// Called when the game starts or when spawned
void AEventListener::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld() != NULL) {
		GetWorld()->GetGameState<APerennialGameStateBase>()->eventListenerSignUp(this);
	}
}

void AEventListener::processEvent() {

}