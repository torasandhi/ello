// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/WaveData.h"
#include "GameFramework/GameModeBase.h"
#include "rglkGameMode.generated.h"

class UDataTable;

UCLASS()
class CPP_API ArglkGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Waves")
	UDataTable* WaveDataTable;
	UPROPERTY(EditDefaultsOnly, Category = "Waves")
	float SpawnRate;
	UPROPERTY(EditDefaultsOnly, Category = "Waves")
	float TimeBetweenWaves = 3.f;
	UPROPERTY(VisibleAnywhere, Category = "Waves")
	TArray<FEnemyDefinition> SpawnQueue;
	UPROPERTY(VisibleAnywhere, Category = "Waves")
	TArray<ArglkEnemyCharacter*> SpawnedEnemiesList;
	UPROPERTY(Transient)
	TArray<AActor*> SpawnPoints;

	UPROPERTY(VisibleAnywhere, Category = "Waves")
	int32 CurrentWaveIndex = 1;
	FTimerHandle SpawnTimer;
	FTimerHandle WaveTransitionTimer;

	void StartWave();
	void SpawnNextEnemy();
	void StopWave();
	void TryStartWaveTransition();
};
