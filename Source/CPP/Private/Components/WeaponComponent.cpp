// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponComponent.h"

#include "rglkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	BaseDamage = GetOwner<ArglkCharacter>()->BaseDamage;
}



void UWeaponComponent::PerformAttack()
{
	ArglkCharacter* Owner = GetOwner<ArglkCharacter>();
	if (!Owner) return;

	USceneComponent* FirePoint = Owner->FirePointComponent;
	FVector Start = FirePoint
		                ? FirePoint->GetComponentLocation() + FVector(0.f, 0.f, 50.f)
		                : Owner->GetActorLocation() + FVector(0.f, 0.f, 50.f);
	FVector Forward = FirePoint ? FirePoint->GetForwardVector() : Owner->GetActorForwardVector();
	FVector End = Start + (Forward * AttackRange);

	TArray<FHitResult> ActorsHit;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Owner);
	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		Start,
		End,
		100.f,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		ActorsHit,
		true
	);

	TArray<AActor*> DamagedActors;
	for (const FHitResult& Hit : ActorsHit)
	{
		if (!bHit)
			return;
		if (Hit.GetActor() && DamagedActors.Contains(Hit.GetActor()))
			continue;
		if (Hit.GetActor()->GetClass()->IsChildOf(Owner->GetClass()))
			continue;

		UGameplayStatics::ApplyDamage(
			Hit.GetActor(),
			BaseDamage,
			Owner->GetInstigatorController(),
			Owner,
			UDamageType::StaticClass()
		);
		DamagedActors.Add(Hit.GetActor());
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName());
	}

	Owner->PlayAttackEffects();
}
