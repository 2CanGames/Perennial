// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "PlantActor.h"
#include "ComposterActor.h"
#include "PlotBuyingActor.h"
#include "CharacterActor.h"
#include "TP_ThirdPersonCharacter.generated.h"

UCLASS(config=Game)
class ATP_ThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxCollider;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	ATP_ThirdPersonCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, USoundCue*> ActionSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAudioComponent* PlayerActionAudio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector BoxColliderSize;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* Other, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(class UPrimitiveComponent* HitComp, class AActor* Other, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	/** Actions mapped to buttons */
	void Harvest();
	void Water();
	void Fertilize();

	/** Action not mapped to buttons */
	UFUNCTION(BlueprintCallable, Category = "Plant")
	bool Plant(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable)
	void AddToCompostList(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable)
	void RemoveFromCompostList(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable)
	void ClearCompostList();

	UFUNCTION(BlueprintCallable)
	void CancelCompost();

	UFUNCTION(BlueprintCallable)
	void AddToPlotBuyingList(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable)
	void RemoveFromPlotBuyingList(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable)
	void ClearPlotBuyingList();

	UFUNCTION(BlueprintCallable)
	void CancelPlotBuying();

	UFUNCTION(BlueprintCallable)
	int GetTotalPlotBuyingPoints();

	ACharacterActor* MyActor;

	UFUNCTION(BlueprintCallable)
	void AddFertilizer();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString InterfacingWith;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlantActor* CurrentPlant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AComposterActor* Composter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlotBuyingActor* PlotBuyer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AInventoryItem*> CompostList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AInventoryItem*> PlotBuyingList;

	virtual void BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintNativeEvent, Category = "Player Actions")
		void OnFertilizerUpdate(int newCount);

		void OnFertilizerUpdate_Implementation(int newCount);

};

