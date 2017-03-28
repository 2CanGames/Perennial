// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Perennial.h"
#include "TP_ThirdPerson.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "TP_ThirdPersonCharacter.h"
#include "PlantActor.h"

//////////////////////////////////////////////////////////////////////////
// ATP_ThirdPersonCharacter

ATP_ThirdPersonCharacter::ATP_ThirdPersonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATP_ThirdPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATP_ThirdPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATP_ThirdPersonCharacter::MoveRight);

	PlayerInputComponent->BindAction("Harvest", IE_Pressed, this, &ATP_ThirdPersonCharacter::Harvest);
	PlayerInputComponent->BindAction("Water", IE_Pressed, this, &ATP_ThirdPersonCharacter::Water);
	PlayerInputComponent->BindAction("Fertilize", IE_Pressed, this, &ATP_ThirdPersonCharacter::Fertilize);
	PlayerInputComponent->BindAction("Plant", IE_Pressed, this, &ATP_ThirdPersonCharacter::Plant);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATP_ThirdPersonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATP_ThirdPersonCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATP_ThirdPersonCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATP_ThirdPersonCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATP_ThirdPersonCharacter::OnResetVR);
}


void ATP_ThirdPersonCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATP_ThirdPersonCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ATP_ThirdPersonCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ATP_ThirdPersonCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATP_ThirdPersonCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATP_ThirdPersonCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATP_ThirdPersonCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ATP_ThirdPersonCharacter::Harvest()
{
	APlantActor* Plant = PlantInRange();
	if (Plant != nullptr)
	{
		// Check if plant is harvestable
		if (Plant->GetStage() == EPlantStage::GROWN)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Harvest!"));
			}

			// Call plant's harvest method
			Plant->Harvest();
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Not harvestable!"));
			}
		}
	}
}

void ATP_ThirdPersonCharacter::Water()
{
	APlantActor* Plant = PlantInRange();
	if (Plant != nullptr)
	{
		// Check if plant is waterable
		if (!(Plant->bIsWatered))
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Water!"));
			}

			// Call plant's water method
			// Plant->Water();
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Already watered!"));
			}
		}
	}
}

void ATP_ThirdPersonCharacter::Fertilize()
{
	APlantActor* Plant = PlantInRange();
	if (Plant != nullptr)
	{
		// Check if player has fertilizer

		// Check if plant is already fertilized
		if (!(Plant->bIsFertilized))
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Fertilize!"));
			}

			// Call plant's fertilize method
			// Plant->Fertilize();

			// Deduct 1 fertilizer from player inventory?
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Already fertilized!"));
			}
		}
	}
}

void ATP_ThirdPersonCharacter::Plant()
{
	APlantActor* Plant = PlantInRange();
	if (Plant != nullptr)
	{
		// Check if player has seeds

		// Check if the dirt mound already has a plant
		if (Plant->GetStage() == EPlantStage::NO_PLANT)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Plant!"));
			}

			// Call plant's plant seed method
			//Plant->Plant(Inventory item seed);

			// Deduct seeds from player inventory?
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Already a plant there!"));
			}
		}
	}
}

APlantActor* ATP_ThirdPersonCharacter::PlantInRange()
{
	TArray<AActor*> Plant;

	// Check if there is a plant in range
	GetOverlappingActors(Plant, APlantActor::StaticClass());
	APlantActor* p = (APlantActor*) Plant.GetData();

	return p;
}