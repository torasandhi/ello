// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/rglkEnemyCharacter.h"
#include "Engine/DataTable.h"
#include "WaveData.generated.h"

USTRUCT(BlueprintType)
struct FEnemyDefinition
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ArglkEnemyCharacter> Enemies;
	UPROPERTY(EditDefaultsOnly)
	float BaseHealth = 50.f;
	UPROPERTY(EditDefaultsOnly)
	float BaseDamage = 10.f;
	UPROPERTY(EditDefaultsOnly)
	int32 SpawnCount = 5;
};

USTRUCT(BlueprintType)
struct FWaveData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FEnemyDefinition> Enemies;
};
