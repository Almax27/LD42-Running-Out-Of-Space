// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerSoul.generated.h"

class USoulTargetComponent;

UENUM(BlueprintType)
enum class EPlayerSoulState : uint8
{
    None = 0,
    Following,
    Thrown,
    ThrownCritical,
    Returning
};

UCLASS()
class RUNNINGOUTOFSPACE_API APlayerSoul : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerSoul();

public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerSoul")
    TArray<TEnumAsByte<EObjectTypeQuery>> ThrowCollisionObjects;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerSoul")
    float ThrowSpeedMax;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerSoul")
    float ThrowAccelerationTime;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerSoul")
    float ThrowDistanceMax;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerSoul")
    float ThrowCriticalDistance;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerSoul")
    float EnergyRegenTime;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerSoul")
    float ReturnDelay;

protected:

    EPlayerSoulState CurrentState = EPlayerSoulState::Following;
    float EnterStateTime = 0.0f;

    TWeakObjectPtr<USoulTargetComponent> SoulTarget = nullptr;

    float CurrentEnergy = 1.0f;

    FVector FollowVelocity = FVector::ZeroVector;
    FVector LastTargetPosition = FVector::ZeroVector;
    FVector Overshoot = FVector::ZeroVector;
    FVector OvershootVelocity = FVector::ZeroVector;
    FRotator RotationalVelocity = FRotator::ZeroRotator;

    FVector ThrowDirection = FVector::ZeroVector;
    float ThrowSpeed = 0;
    float ThrowEnergy = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

    UFUNCTION(BlueprintCallable, Category = "PlayerSoul")
    void SetSoulTarget(USoulTargetComponent* NewTarget);

    UFUNCTION(BlueprintCallable, Category = "PlayerSoul")
    bool Throw(const FVector& InDirection);

    UFUNCTION(BlueprintCallable, Category = "PlayerSoul")
    bool UseThrow();

    UFUNCTION(BlueprintPure, Category = "PlayerSoul")
    EPlayerSoulState GetCurrentState() const { return CurrentState; }

    UFUNCTION(BlueprintPure, Category = "PlayerSoul")
    bool IsCritical() const { return CurrentState == EPlayerSoulState::ThrownCritical; }

    UFUNCTION(BlueprintPure, Category = "PlayerSoul")
    USoulTargetComponent* GetSoulTarget() const { return SoulTarget.Get(); }

protected:

    void SetState(EPlayerSoulState NewState);

    UFUNCTION(BlueprintNativeEvent, Category = "PlayerSoul")
    void OnEnterState(EPlayerSoulState NewState);
    virtual void OnEnterState_Implementation(EPlayerSoulState NewState);

    UFUNCTION(BlueprintNativeEvent, Category = "PlayerSoul")
    void OnExitState(EPlayerSoulState NewState);
    virtual void OnExitState_Implementation(EPlayerSoulState NewState);

    UFUNCTION(BlueprintImplementableEvent, Category = "PlayerSoul")
    void OnTargetChanged(USoulTargetComponent* NewTarget);

    virtual void TickFollowing(float DeltaTime);
    virtual void TickThrown(float DeltaTime);
	
};
