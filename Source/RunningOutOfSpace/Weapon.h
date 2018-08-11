// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS(Blueprintable)
class RUNNINGOUTOFSPACE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

    virtual void SetOwner(AActor* NewOwner) override;

    UFUNCTION(BlueprintCallable, Category = Weapon)
    virtual void StartUse();

    UFUNCTION(BlueprintCallable, Category = Weapon)
    virtual void EndUse();

protected:

    UFUNCTION(BlueprintImplementableEvent, Category = Weapon)
    void OnOwnerChanged();

    UFUNCTION(BlueprintImplementableEvent, Category = Weapon)
    void OnStartUse();

    UFUNCTION(BlueprintImplementableEvent, Category = Weapon)
    void OnEndUse();

public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
    FName DefaultAttachSocket;
	
protected:

};
