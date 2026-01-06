#include "rglkGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "ObjectPoolSubsystem.h"

void ArglkGameMode::StartWave()
{
	GetWorld()->GetTimerManager().ClearTimer(WaveTransitionTimer);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), SpawnPoints);

	if (SpawnPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Spawn Points found in level!"));
		return;
	}

	if (!WaveDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("WaveDataTable is not found"));
		return;
	}

	const FName RowName = FName(*FString::FromInt(CurrentWaveIndex));
	if (FWaveData* WaveRow = WaveDataTable->FindRow<FWaveData>(RowName, TEXT("WaveContext")))
	{
		SpawnQueue.Empty();

		for (const FEnemyDefinition& Config : WaveRow->Enemies)
		{
			for (int32 i = 0; i < Config.SpawnCount; i++)
			{
				SpawnQueue.Add(Config);
			}
		}

		GetWorld()->GetTimerManager().SetTimer(
			SpawnTimer,
			this,
			&ArglkGameMode::SpawnNextEnemy,
			SpawnRate,
			true
		);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No more waves defined in DataTable. Victory?"));
	}
}

void ArglkGameMode::SpawnNextEnemy()
{
	if (SpawnQueue.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No available enemies in queue"));
		return;
	}
	FEnemyDefinition EnemyData = SpawnQueue[0];
	SpawnQueue.RemoveAt(0);

	FTransform SpawnTransform = FTransform::Identity;
	if (SpawnPoints.Num() > 0)
	{
		int32 RandIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
		if (SpawnPoints[RandIndex])
		{
			SpawnTransform = SpawnPoints[RandIndex]->GetActorTransform();
		}
	}

	if (UObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
	{
		AActor* SpawnedActor = PoolSubsystem->GetActorFromPool(EnemyData.Enemies, SpawnTransform.GetLocation(),
		                                                       SpawnTransform.Rotator());

		if (ArglkEnemyCharacter* SpawnedEnemy = static_cast<ArglkEnemyCharacter*>(SpawnedActor))
		{
			if (!SpawnedEnemy->Implements<UPoolableInterface>()) return;
			IPoolableInterface::Execute_OnSpawnFromPool(SpawnedEnemy);
			SpawnedEnemiesList.Add(SpawnedEnemy);
		}
	}
}

void ArglkGameMode::TryStartWaveTransition()
{
	if (SpawnedEnemiesList.Num() != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemies are still alive!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Starting Wave Transition"));
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
	CurrentWaveIndex++;
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimer,
		this,
		&ArglkGameMode::StartWave,
		TimeBetweenWaves,
		false
	);
}

void ArglkGameMode::StopWave() {}
