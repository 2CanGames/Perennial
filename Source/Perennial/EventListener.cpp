// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "EventListener.h"
#include "TimeController.h"

EventListener::EventListener() {
	ATimeController::getInstance()->eventListenerSignUp(this);
}