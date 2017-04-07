// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "EventListener.h"

#include "PerennialGameStateBase.h"

//#include "TimeController.h"

AEventListener::AEventListener() {
	
	if (GetWorld() != NULL) {
		//static_cast<PerennialGameStateBase>(GetWorld()->GetGameState());
		GetWorld()->GetGameState<APerennialGameStateBase>();
	}
	else {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(
				GEngine->ScreenMessages.Num() + 1,
				6.0f,
				FColor::Green,
				"GetWorld didnt work"
			);
	}
	//ATimeController::getInstance()->eventListenerSignUp(this);
}

AEventListener::~AEventListener() {
		//ATimeController::getInstance()->eventListenerRemove(this);
}

// Called when the game starts or when spawned
void AEventListener::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld() != NULL) {
		//static_cast<PerennialGameStateBase>(GetWorld()->GetGameState());
		GetWorld()->GetGameState<APerennialGameStateBase>()->eventListenerSignUp(this);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(
				GEngine->ScreenMessages.Num() + 1,
				6.0f,
				FColor::Green,
				"GetWorld WORKED"
			);
	}
	else {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(
				GEngine->ScreenMessages.Num() + 1,
				6.0f,
				FColor::Green,
				"GetWorld didnt work"
			);
	}
}

void AEventListener::processEvent() {

}