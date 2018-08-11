// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

    //Ensure we're attached, as SetOwner can be called before we have a root component
    if(AActor* CurrentOwner = GetOwner())
    {
        AttachToActor(CurrentOwner, FAttachmentTransformRules::SnapToTargetNotIncludingScale, DefaultAttachSocket);
    }
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::SetOwner(AActor* NewOwner)
{
    AActor* CurrentOwner = GetOwner();
    Super::SetOwner(NewOwner);
    if(CurrentOwner != NewOwner)
    {
        if(NewOwner)
        {
            AttachToActor(NewOwner, FAttachmentTransformRules::SnapToTargetNotIncludingScale, DefaultAttachSocket);
        }
        else
        {
            DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        }
        OnOwnerChanged();
    }
}

void AWeapon::StartUse()
{
    OnStartUse();
}

void AWeapon::EndUse()
{
    OnEndUse();
}

    

