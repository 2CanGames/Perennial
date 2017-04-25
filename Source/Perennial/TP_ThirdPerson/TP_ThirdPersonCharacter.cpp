// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Perennial.h"
#include "TP_ThirdPerson.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "TP_ThirdPersonCharacter.h"
#include "PlantActor.h"
#include "ComposterActor.h"
#include "PlotBuyingActor.h"
#include "CharacterActor.h"

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

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollider->SetupAttachment(RootComponent);
	BoxCollider->InitBoxExtent(BoxColliderSize);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ATP_ThirdPersonCharacter::OnBeginOverlap);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &ATP_ThirdPersonCharacter::OnEndOverlap);
}

void ATP_ThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterActor::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0) 
	{
		MyActor = (ACharacterActor*)FoundActors[0];
	}
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


void ATP_ThirdPersonCharacter::OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* Other, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, *HitComp->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, *Other->GetName());
	}

	if (Other->IsA(APlantActor::StaticClass()))
	{
		CurrentPlant = (APlantActor*)Other;
	}
	else if (Other->IsA(AComposterActor::StaticClass()))
	{
		Composter = (AComposterActor*)Other;
	}
	else if (Other->IsA(APlotBuyingActor::StaticClass()))
	{
		PlotBuyer = (APlotBuyingActor*)Other;
	}
}

void ATP_ThirdPersonCharacter::OnEndOverlap(class UPrimitiveComponent* HitComp, class AActor* Other, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((APlantActor*)Other == CurrentPlant)
	{
		CurrentPlant = nullptr;
	}
	else if (Other->IsA(AComposterActor::StaticClass()))
	{
		Composter = nullptr;
	}
	else if (Other->IsA(APlotBuyingActor::StaticClass()))
	{
		PlotBuyer = nullptr;
	}
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
	if (CurrentPlant)
	{
		// Check if plant is harvestable
		if (CurrentPlant->GetStage() == EPlantStage::GROWN && CurrentPlant->bIsHarvestable)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Harvest!"));
			}

			// Pass control to Character Actor
			MyActor->Harvest(CurrentPlant);
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Not harvestable!"));
			}

			GEngine->GetFirstLocalPlayerController(GetWorld())->PlayDynamicForceFeedback(0.5f, 0.25f, true, true, true, true, EDynamicForceFeedbackAction::Start, FLatentActionInfo());
		}
	}
}

void ATP_ThirdPersonCharacter::Water()
{
	if (CurrentPlant)
	{
		// Check if plant is waterable
		if (!(CurrentPlant->bIsWatered))
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Water!"));
			}

			// Call plant's water method
			CurrentPlant->Water();
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
	if (CurrentPlant)
	{
		// Check if plant is already fertilized
		if (!(CurrentPlant->bIsFertilized))
		{
			// Check that player has fertilizer
			if (!MyActor->DeleteFertilizer())
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Not enough fertilizer"));
				}
			}
			else 
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Fertilize!"));
				}

				// Call plant's fertilize method
				CurrentPlant->Fertilize();
			}
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

bool ATP_ThirdPersonCharacter::Plant(AInventoryItem* Item)
{
	if (!Item->getIsSeed())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Not a seed!"));
		}

		return false;
	}

	if (CurrentPlant)
	{
		// Check if the dirt mound already has a plant
		if (CurrentPlant->GetStage() == EPlantStage::NO_PLANT)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Plant!"));
			}

			// Pass to Character Actor
			MyActor->PlantSeed(CurrentPlant, Item);
			return true;
		}
	}

	return false;
}

// Adds item to compost list
// Does NOT remove from inventory yet
void ATP_ThirdPersonCharacter::AddToCompostList(AInventoryItem * Item)
{
	if (CompostList.Contains(Item))
	{
		RemoveFromCompostList(Item);
	}
	else
	{
		CompostList.Add(Item);
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("ADD - # Items in CompostList: ") + FString::FromInt(CompostList.Num()));
	}
}

// For 'unclicking' an item when composting
// Only removes from compost list
void ATP_ThirdPersonCharacter::RemoveFromCompostList(AInventoryItem * Item)
{
	int count = 0;
	for (auto& CurrentItem : CompostList)
	{
		if (CurrentItem == Item)
		{
			CompostList.RemoveAt(count);
			return;
		}
		count++;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("REMOVE - # Items in CompostList: ") + FString::FromInt(CompostList.Num()));
	}
}

// When player has confirmed a compost
// Removes items from compost list AND removes all items from inventory
void ATP_ThirdPersonCharacter::ClearCompostList()
{
	for (auto& CurrentItem : CompostList)
	{
		MyActor->PlayerInventory->removeItemToInventory(CurrentItem);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Item removed from inventory"));
		}
	}

	CompostList.Empty();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("SUCCESSFUL COMPOST - # Items in CompostList: ") + FString::FromInt(CompostList.Num()));
	}
}

// Player cancelled in the middle of composting
// Removes all items from compost list
void ATP_ThirdPersonCharacter::CancelCompost()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("CANCEL - # Items in CompostList: ") + FString::FromInt(CompostList.Num()));
	}

	CompostList.Empty();
}

// Adds item to plot buying list
// Does NOT remove from inventory yet
void ATP_ThirdPersonCharacter::AddToPlotBuyingList(AInventoryItem * Item)
{
	if (PlotBuyingList.Contains(Item))
	{
		RemoveFromPlotBuyingList(Item);
	}
	else
	{
		PlotBuyingList.Add(Item);
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("ADD - # Items in PlotBuyingList: ") + FString::FromInt(PlotBuyingList.Num()));
	}
}

// For 'unclicking' an item when buying plot
// Only removes from plot buying list
void ATP_ThirdPersonCharacter::RemoveFromPlotBuyingList(AInventoryItem * Item)
{
	int count = 0;
	for (auto& CurrentItem : PlotBuyingList)
	{
		if (CurrentItem == Item)
		{
			PlotBuyingList.RemoveAt(count);
			return;
		}
		count++;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("REMOVE - # Items in PlotBuyingList: ") + FString::FromInt(PlotBuyingList.Num()));
	}
}

// When player has confirmed buying plot
// Removes items from plot buying list AND removes all items from inventory
void ATP_ThirdPersonCharacter::ClearPlotBuyingList()
{
	for (auto& CurrentItem : PlotBuyingList)
	{
		MyActor->PlayerInventory->removeItemToInventory(CurrentItem);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Item removed from inventory"));
		}
	}

	PlotBuyingList.Empty();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("SUCCESSFUL PLOT BUY - # Items in PlotBuyingList: ") + FString::FromInt(PlotBuyingList.Num()));
	}
}

// Player cancelled in the middle of buying plot
// Removes all items from plot buying list
void ATP_ThirdPersonCharacter::CancelPlotBuying()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("CANCEL - # Items in PlotBuyingList: ") + FString::FromInt(PlotBuyingList.Num()));
	}

	PlotBuyingList.Empty();
}

int ATP_ThirdPersonCharacter::GetTotalPlotBuyingPoints()
{
	int TotalPoints = 0;

	for (auto& CurrentItem : PlotBuyingList)
	{
		TotalPoints += CurrentItem->getQuality();
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Plot Buying Points so far: ") + FString::FromInt(TotalPoints));
	}

	return TotalPoints;
}

void ATP_ThirdPersonCharacter::AddFertilizer()
{
	MyActor->NumFertilizers++;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Fertilizers: ") + FString::FromInt(MyActor->NumFertilizers));
	}
}
