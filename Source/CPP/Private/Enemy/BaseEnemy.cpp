// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseEnemy.h"

#include "ObjectPoolSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

class UObjectPoolSubsystem;
// Sets default values
ABaseEnemy::ABaseEnemy() : stopDistance(0), target(nullptr)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseEnemy::GetTarget()
{
	// 1. Just grab the player directly. No tags, no searching.
	target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ABaseEnemy::LookAtTarget(float DeltaTime)
{
	if (!target)
	{
		UE_LOG(LogTemp, Error, TEXT("target is null %s"));
		return;
	}

	FVector start = GetActorLocation();
	FVector destination = target->GetActorLocation();
	FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(start, destination);

	FRotator newRotation = FMath::RInterpConstantTo(
		GetActorRotation(),
		lookAtRotation,
		DeltaTime,
		TurnSpeed
	);

	SetActorRotation(newRotation);
}

void ABaseEnemy::MoveTowardsTarget(float DeltaTime)
{
	if (!target) return;

	float distance = FVector::Dist(GetActorLocation(), target->GetActorLocation());

	if (distance > stopDistance)
	{
		FVector forwardDirection = GetActorForwardVector();
		FVector deltaLocation = forwardDirection * MoveSpeed;

		AddActorWorldOffset(deltaLocation, true);
	}
}

void ABaseEnemy::OnDeath()
{
	UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	if (Pool)
	{
		Pool->ReturnActorToPool(this);
	}
	else
	{
		Destroy();
	}
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (target)
	{
		LookAtTarget(DeltaTime);
		MoveTowardsTarget(DeltaTime);
	}
	else
	{
		GetTarget();
	}
}

void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
