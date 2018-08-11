// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

#include "Engine/World.h"
#include "PlayerSoul.h"
#include "SoulTargetComponent.h"

FName APlayerCharacter::IdleSoulTargetName(TEXT("IdleSoulTarget"));
FName APlayerCharacter::AimSoulTargetName(TEXT("AimSoulTarget"));

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    IdleSoulTarget = CreateDefaultSubobject<USoulTargetComponent>(APlayerCharacter::IdleSoulTargetName);
    if (IdleSoulTarget)
    {
        IdleSoulTarget->SetupAttachment(RootComponent);
    }

    AimSoulTarget = CreateDefaultSubobject<USoulTargetComponent>(APlayerCharacter::AimSoulTargetName);
    if (AimSoulTarget)
    {
        AimSoulTarget->SetupAttachment(RootComponent);
    }
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    UWorld* World = GetWorld();
    check(World);

    if (PlayerSoulClass)
    {
        FActorSpawnParameters SpawnInfo;
        PlayerSoulInstance = World->SpawnActor<APlayerSoul>(PlayerSoulClass, IdleSoulTarget->GetComponentTransform(), SpawnInfo);
        PlayerSoulInstance->SetSoulTarget(IdleSoulTarget);
    }
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

