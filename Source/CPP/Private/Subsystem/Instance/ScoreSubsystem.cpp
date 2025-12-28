// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/Instance/ScoreSubsystem.h"

float UScoreSubsystem::GetScore() const
{
	return CurrentScore;
}

float UScoreSubsystem::SetScore(float amount)
{
	CurrentScore += amount;
	OnScoreChanged.Broadcast(CurrentScore);
	return CurrentScore;
}
