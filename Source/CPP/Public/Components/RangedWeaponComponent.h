// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WeaponComponent.h"
#include "RangedWeaponComponent.generated.h"

/**
 * 
 */
class UArrowComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPP_API URangedWeaponComponent : public UWeaponComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void PerformAttack() override;

	UPROPERTY(EditDefaultsOnly, Category="Weapons")
	TSubclassOf<AActor> Projectile;
	UPROPERTY(EditDefaultsOnly, Category="Weapons")
	float TraceDistance;

private:
	UPROPERTY()
	TArray<AActor*> SpawnedProjectiles;
	void HandleActorReturnToPool(AActor* Projectile);
};
