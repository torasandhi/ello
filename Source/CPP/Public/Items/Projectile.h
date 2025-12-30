// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPoolableInterface.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class CPP_API AProjectile : public AActor, public IPoolableInterface
{
	GENERATED_BODY()

public:
	AProjectile();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	virtual void Activate(const FVector& InDirection, AActor* Owner);
	virtual void OnSpawnFromPool_Implementation() override;
	virtual FOnReturnedToPool& OnReturnedToPool() override;
	UFUNCTION()
	virtual void DealDamage(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void ReturnActorToPool();

	UPROPERTY()
	USphereComponent* SphereCollider;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float BaseDamage = 10.f;
	UPROPERTY(EditDefaultsOnly, Category="Projectile Settings")
	float Speed = 6000.f;
	UPROPERTY(EditDefaultsOnly, Category="Projectile Settings")
	float LifeTime = 2.f;
	float AliveTime = 0.f;

protected:

	FVector MoveDirection = FVector::ZeroVector;
	FOnReturnedToPool ReturnToPool;
};
