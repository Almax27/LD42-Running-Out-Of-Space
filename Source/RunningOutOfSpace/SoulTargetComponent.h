// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SoulTargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUNNINGOUTOFSPACE_API USoulTargetComponent : public USceneComponent
{
	GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SoulTarget")
    bool EnableTrail = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SoulTarget")
    float FollowSmoothTime;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SoulTarget")
    float OvershootSmoothTime;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SoulTarget")
    float TurnSmoothTime;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SoulTarget")
    float RiseRate;
};
