// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Projectile.h"
#include "ObjectPoolSubsystem.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::Activate(const FVector& Direction, AActor* InOwner)
{
	SetOwner(InOwner);                 
	MoveDirection = Direction.GetSafeNormal();
	AliveTime = 0.f;
}

void AProjectile::OnSpawnFromPool_Implementation()
{
	IPoolableInterface::OnSpawnFromPool_Implementation();
}

IPoolableInterface::FOnReturnedToPool& AProjectile::OnReturnedToPool()
{
	return ReturnToPool;
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AliveTime += DeltaTime;
	if (AliveTime >= LifeTime)
	{
		if (UObjectPoolSubsystem* Pool =
			GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
		{
			Pool->ReturnActorToPool(this);
		}
		return;
	}

	const FVector Start = GetActorLocation();
	const FVector End = Start + MoveDirection * Speed * DeltaTime;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params
	))
	{
		SetActorLocation(Hit.ImpactPoint);

		if (UObjectPoolSubsystem* Pool =
			GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
		{
			Pool->ReturnActorToPool(this);
		}
		return;
	}

	SetActorLocation(End);
}


// OnOverlapBegin -> ApplyDamage -> Return To Pool
