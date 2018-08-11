// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSoul.h"

#include "Engine/World.h"
#include "Math.h"

#include "SoulTargetComponent.h"

PRAGMA_DISABLE_OPTIMIZATION

// Sets default values
APlayerSoul::APlayerSoul()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = ETickingGroup::TG_PostPhysics;

    ThrowSpeedMax = 1000.0f;
    ThrowAccelerationTime = 0.3f;
    ThrowDistanceMax = 1000.0f;
    ThrowCriticalDistance = 200.0f;
    EnergyRegenTime = 2.0f;
    ReturnDelay = 0.2f;

}

// Called when the game starts or when spawned
void APlayerSoul::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerSoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    UWorld* World = GetWorld();
    check(World);

    switch (CurrentState)
    {
    case EPlayerSoulState::Following:
        TickFollowing(DeltaTime);
        break;
    case EPlayerSoulState::Thrown:
    case EPlayerSoulState::ThrownCritical:
        TickThrown(DeltaTime);
        break;
    case EPlayerSoulState::Returning:
        if (World->TimeSeconds > EnterStateTime + ReturnDelay)
        {
            SetState(SoulTarget.IsValid() ? EPlayerSoulState::Following : EPlayerSoulState::None);
        }
        break;
    default:
        SetState(SoulTarget.IsValid() ? EPlayerSoulState::Following : EPlayerSoulState::None);
    }

    if (CurrentState == EPlayerSoulState::Following)
    {
        if (EnergyRegenTime > 0)
        {
            CurrentEnergy = FMath::Clamp(CurrentEnergy + DeltaTime / EnergyRegenTime, 0.0f, 1.0f);
        }
        else
        {
            CurrentEnergy = 1.0f;
        }
    }
}

// Called to bind functionality to input
void APlayerSoul::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerSoul::SetSoulTarget(USoulTargetComponent* NewTarget)
{
    if (SoulTarget != NewTarget)
    {
        if (SoulTarget.IsValid())
        {
            //Unregister Soul target owner as tick Prerequisite
            if (AActor* Actor = SoulTarget->GetOwner())
            {
                PrimaryActorTick.RemovePrerequisite(Actor, Actor->PrimaryActorTick);
            }
        }

        SoulTarget = NewTarget;

        if (SoulTarget.IsValid())
        {
            //Register Soul target owner as tick Prerequisite to make sure we tick after it - for following
            if (AActor* Actor = SoulTarget->GetOwner())
            {
                PrimaryActorTick.AddPrerequisite(Actor, Actor->PrimaryActorTick);
            }
        }

        FollowVelocity = FVector::ZeroVector;
        if(SoulTarget.IsValid())
        {
            LastTargetPosition = SoulTarget->GetComponentLocation();
        }
        Overshoot = FVector::ZeroVector;
        OvershootVelocity = FVector::ZeroVector;
        RotationalVelocity = FRotator::ZeroRotator;

        OnTargetChanged(NewTarget);
    }
}

bool APlayerSoul::Throw(const FVector& InThrowDirection)
{
    if (CurrentEnergy > 0 && CurrentState < EPlayerSoulState::Thrown)
    {
        ThrowDirection = InThrowDirection.GetSafeNormal();
        SetState(EPlayerSoulState::Thrown);
        return true;
    }
    return false;
}

bool APlayerSoul::UseThrow()
{
    if (CurrentState >= EPlayerSoulState::Thrown)
    {
        //consume energy
        CurrentEnergy = ThrowEnergy;
        SetState(EPlayerSoulState::Returning);
        return true;
    }
    return false;

}

void APlayerSoul::SetState(EPlayerSoulState NewState)
{
    if (NewState == CurrentState)
        return;

    UWorld* World = GetWorld();
    check(World);

    OnExitState(CurrentState);
    CurrentState = NewState;
    EnterStateTime = World->TimeSeconds;
    OnEnterState(NewState);
}

void APlayerSoul::OnEnterState_Implementation(EPlayerSoulState State)
{
    switch (State)
    {
    case EPlayerSoulState::None:
        break;
    case EPlayerSoulState::Following:
        FollowVelocity = FVector::ZeroVector;
        if (SoulTarget.IsValid())
        {
            LastTargetPosition = SoulTarget->GetComponentLocation();
        }
        Overshoot = FVector::ZeroVector;
        OvershootVelocity = FVector::ZeroVector;
        RotationalVelocity = FRotator::ZeroRotator;
        break;
    case EPlayerSoulState::Thrown:
        ThrowSpeed = 0.0f;
        ThrowEnergy = CurrentEnergy;
        break;
    case EPlayerSoulState::ThrownCritical:
        break;
    case EPlayerSoulState::Returning:
        break;
    }
}

void APlayerSoul::OnExitState_Implementation(EPlayerSoulState State)
{
    switch (State)
    {
    case EPlayerSoulState::None:
        break;
    case EPlayerSoulState::Following:
        break;
    case EPlayerSoulState::Thrown:
        break;
    case EPlayerSoulState::ThrownCritical:
        break;
    case EPlayerSoulState::Returning:
        if (SoulTarget.IsValid())
        {
            SetActorLocation(SoulTarget->GetComponentLocation());
        }
        break;
    }
}

void APlayerSoul::TickFollowing(float DeltaTime)
{
    if (SoulTarget.IsValid())
    {
        FVector Location = GetActorLocation();
        FVector TargetLocation = SoulTarget->GetComponentLocation();
        TargetLocation += FVector::UpVector * (FVector::Dist(Location, TargetLocation) * SoulTarget->RiseRate);
        FVector TargetVelocity = (TargetLocation - LastTargetPosition) / DeltaTime;
        LastTargetPosition = TargetLocation;

        //overshoot target relative to smooth damping so we work towards velocity synchronisation
        Overshoot = VSmoothDamp(Overshoot, TargetVelocity * SoulTarget->FollowSmoothTime, OvershootVelocity, SoulTarget->OvershootSmoothTime, FLT_MAX, DeltaTime);

        //smooth position
        Location = VSmoothDamp(Location, TargetLocation + Overshoot, FollowVelocity, SoulTarget->FollowSmoothTime, FLT_MAX, DeltaTime);

        FRotator Rotation = GetActorRotation();
        FRotator TargetRotation = SoulTarget->GetComponentRotation();
        Rotation = RSmoothDamp(Rotation, TargetRotation, RotationalVelocity, SoulTarget->TurnSmoothTime, FLT_MAX, DeltaTime);

        SetActorLocationAndRotation(Location, Rotation);

        //DebugExtension.DebugPoint(targetPosition + overshoot, Color.red, 0.5f, 0, false);
        //DebugExtension.DebugPoint(targetPosition, Color.red, 0.5f, 0, false);
    }
    else
    {
        SetState(EPlayerSoulState::None);
    }
}

void APlayerSoul::TickThrown(float DeltaTime)
{
    UWorld* World = GetWorld();
    check(World);

    float Time = World->TimeSeconds;

    if (ThrowEnergy <= 0 || ThrowDistanceMax <= 0)
    {
        SetState(EPlayerSoulState::Returning);
        return;
    }

    if (ThrowAccelerationTime > 0 && ThrowSpeed < ThrowSpeedMax)
    {
        ThrowSpeed = FMath::Clamp(ThrowSpeed + (ThrowSpeedMax / ThrowAccelerationTime) * DeltaTime, 0.0f, ThrowSpeedMax);
    }
    else
    {
        ThrowSpeed = ThrowSpeedMax;
    }

    FHitResult HitResult;
    FCollisionObjectQueryParams QueryParams(ThrowCollisionObjects);
    const float DistanceToTravel = FMath::Min(ThrowSpeed * DeltaTime, ThrowEnergy * ThrowDistanceMax);
    FVector SimulatedPos = GetActorLocation();
    uint8 Iterations = 5;
    float DistanceTraveled = 0.0f;
    while (Iterations > 0 && !FMath::IsNearlyZero(DistanceToTravel))
    {
        const float RemainingDistance = DistanceToTravel - DistanceTraveled;
        const FVector TargetPos = SimulatedPos + ThrowDirection * RemainingDistance;
        if (World->LineTraceSingleByObjectType(HitResult, SimulatedPos, TargetPos, QueryParams))
        {
            SimulatedPos = HitResult.Location;
            DistanceTraveled += HitResult.Distance;

            bool bShouldReflect = false;
            bool bShouldPassThrough = false;
            if (bShouldReflect)
            {
                ThrowDirection.MirrorByVector(HitResult.Normal);
            }
            else if(!bShouldPassThrough)
            {
                SetState(EPlayerSoulState::Returning);
                return;
            }
        }
        else
        {
            SimulatedPos = TargetPos;
            DistanceTraveled += RemainingDistance;
            break;
        }
        Iterations--;
    }

    //Apply result of simulation
    SetActorLocation(SimulatedPos);

    //Consume energy
    ThrowEnergy = FMath::Clamp(ThrowEnergy - DistanceTraveled / ThrowDistanceMax, 0.0f, 1.0f);

    if (ThrowEnergy <= 0)
    {
        SetState(EPlayerSoulState::Returning);
        return;
    }

    float ThrowDistanceRemaining = ThrowEnergy * ThrowDistanceMax;
    if (!IsCritical() && ThrowDistanceRemaining <= ThrowCriticalDistance)
    {
        SetState(EPlayerSoulState::ThrownCritical);
    }
}

PRAGMA_ENABLE_OPTIMIZATION
