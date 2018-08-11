// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

static FORCEINLINE
float FSmoothDamp(float Current, float Target, float& CurrentVelocity, float SmoothTime, float MaxSpeed, float DeltaTime)
{
    if (SmoothTime < 0.0001f)
        return Target;
    float num = 2.f / SmoothTime;
    float num2 = num * DeltaTime;
    float num3 = 1.f / (1.f + num2 + 0.48f * num2 * num2 + 0.235f * num2 * num2 * num2);
    float num4 = Current - Target;
    float num5 = Target;
    float num6 = MaxSpeed * SmoothTime;
    num4 = FMath::Clamp(num4, -num6, num6);
    Target = Current - num4;
    float num7 = (CurrentVelocity + num * num4) * DeltaTime;
    CurrentVelocity = (CurrentVelocity - num * num7) * num3;
    float num8 = Target + (num4 + num7) * num3;
    if (num5 - Current > 0.f == num8 > num5)
    {
        num8 = num5;
        CurrentVelocity = (num8 - num5) / DeltaTime;
    }
    return num8;
}

static FORCEINLINE
FVector2D VSmoothDamp2D(const FVector2D& Current, const FVector2D& Target, FVector2D& CurrentVelocity, float SmoothTime, float MaxSpeed, float DeltaTime)
{
    if (SmoothTime < 0.0001f)
        return Target;
    FVector2D result;
    result.X = FSmoothDamp(Current.X, Target.X, CurrentVelocity.X, SmoothTime, MaxSpeed, DeltaTime);
    result.Y = FSmoothDamp(Current.Y, Target.Y, CurrentVelocity.Y, SmoothTime, MaxSpeed, DeltaTime);
    return result;
}

static FORCEINLINE
FVector VSmoothDamp(const FVector& Current, const FVector& Target, FVector& CurrentVelocity, float SmoothTime, float MaxSpeed, float DeltaTime)
{
    if (SmoothTime < 0.0001f)
        return Target;
    FVector result;
    result.X = FSmoothDamp(Current.X, Target.X, CurrentVelocity.X, SmoothTime, MaxSpeed, DeltaTime);
    result.Y = FSmoothDamp(Current.Y, Target.Y, CurrentVelocity.Y, SmoothTime, MaxSpeed, DeltaTime);
    result.Z = FSmoothDamp(Current.Z, Target.Z, CurrentVelocity.Z, SmoothTime, MaxSpeed, DeltaTime);
    return result;
}

static FORCEINLINE
FRotator RSmoothDamp(const FRotator& Current, const FRotator& Target, FRotator& CurrentVelocity, float SmoothTime, float MaxSpeed, float DeltaTime)
{
    if (SmoothTime < 0.0001f)
        return Target;
    FRotator result;
    result.Pitch = FSmoothDamp(Current.Pitch, Target.Pitch, CurrentVelocity.Pitch, SmoothTime, MaxSpeed, DeltaTime);
    result.Yaw = FSmoothDamp(Current.Yaw, Target.Yaw, CurrentVelocity.Yaw, SmoothTime, MaxSpeed, DeltaTime);
    result.Roll = FSmoothDamp(Current.Roll, Target.Roll, CurrentVelocity.Roll, SmoothTime, MaxSpeed, DeltaTime);
    return result;
}