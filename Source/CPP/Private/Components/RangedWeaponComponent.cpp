#include "Components/RangedWeaponComponent.h"
#include "ObjectPoolSubsystem.h"
#include "Items/Projectile.h"
#include "Player/rglkPlayerCharacter.h"

void URangedWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URangedWeaponComponent::PerformAttack()
{
	ArglkCharacter* Owner = GetOwner<ArglkCharacter>();
	if (!Owner || !GetWorld()) return;

	USceneComponent* FirePoint = Owner->FirePointComponent;

	const FVector Start = FirePoint ? FirePoint->GetComponentLocation() : Owner->GetActorLocation();
	FVector ForwardDir = FirePoint ? FirePoint->GetForwardVector() : Owner->GetActorForwardVector();
	ForwardDir.Z = 0.f;
	ForwardDir.Normalize();
	const FVector End = Start + ForwardDir * TraceDistance;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f, 0, 3.0f);

	const FVector Target = bHit ? Hit.ImpactPoint : End;
	const FRotator ProjectileRot = (Target - Start).Rotation();

	if (UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
	{
		AActor* SpawnedActor = Pool->GetActorFromPool(Projectile, Start, ProjectileRot);

		if (AProjectile* SpawnedProjectile = static_cast<AProjectile*>(SpawnedActor))
		{
			SpawnedProjectile->Activate(ProjectileRot.Vector(), Owner);
			SpawnedProjectile->OnReturnedToPool().AddUObject(
				this,
				&URangedWeaponComponent::HandleActorReturnToPool
			);

			if (!SpawnedProjectile->Implements<UPoolableInterface>()) return;
			IPoolableInterface::Execute_OnSpawnFromPool(SpawnedProjectile);
		}
		SpawnedProjectiles.Add(SpawnedActor);
	}
}

void URangedWeaponComponent::HandleActorReturnToPool(AActor* SpawnedProjectile)
{
	SpawnedProjectiles.Remove(SpawnedProjectile);
}
