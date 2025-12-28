// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ScoreSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32);

UCLASS()
class CPP_API UScoreSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public :
	float GetScore() const;
	float SetScore(float amount);

	FOnScoreChanged OnScoreChanged;

private:
	int32 CurrentScore = 0;
};
