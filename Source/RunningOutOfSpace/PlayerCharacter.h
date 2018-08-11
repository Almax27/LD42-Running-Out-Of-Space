// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class RUNNINGOUTOFSPACE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

public:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCharacter")
    class USoulTargetComponent* IdleSoulTarget;
    static FName IdleSoulTargetName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCharacter")
    class USoulTargetComponent* AimSoulTarget;
    static FName AimSoulTargetName;

public:

    //TODO: This should be driven by the game mode, not the Character
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter")
    TSubclassOf<class APlayerSoul> PlayerSoulClass;

protected:

    UPROPERTY(VisibleInstanceOnly, Category = "PlayerCharacter")
    class APlayerSoul* PlayerSoulInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

    UFUNCTION(BlueprintPure, Category = "PlayerCharacter")
    class APlayerSoul* GetPlayerSoulInstance() const 
    { return PlayerSoulInstance; }
	
};
