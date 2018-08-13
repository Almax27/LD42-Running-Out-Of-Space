// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

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

    if(AActor* NewOwner = GetOwner())
    {
        UpdateAttachment();
        OnOwnerChanged();
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
    if(HasActorBegunPlay() &&  CurrentOwner != NewOwner)
    {
        if(NewOwner)
        {
            UpdateAttachment();
        }
        else
        {
            DetachRootComponentFromParent();
        }
        OnOwnerChanged();
    }
}

void AWeapon::UpdateAttachment()
{
    ACharacter* OwningCharacter = Cast<ACharacter>(GetOwner());
    if(OwningCharacter && DefaultAttachSocket.IsValid())
    {
        AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, DefaultAttachSocket);
    }
    else
    {
        AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, DefaultAttachSocket);
    }
}

void AWeapon::StartUse()
{
    OnStartUse();
    m_isInUse = true;
}

void AWeapon::EndUse()
{
    OnEndUse();
    m_isInUse = false;
}

bool AWeapon::IsInUse() const
{
    return m_isInUse;
}

    

