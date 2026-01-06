#include "Enemy/rglkEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "ObjectPoolSubsystem.h"
#include "rglkGameMode.h"
#include "Components/WeaponComponent.h"
#include "Subsystem/Instance/ScoreSubsystem.h"

ArglkEnemyCharacter::ArglkEnemyCharacter()
{
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	RangedWeaponComp = CreateDefaultSubobject<URangedWeaponComponent>(TEXT("RangedWeaponComponent"));
	FirePointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointComponent"));
	FirePointComponent->SetupAttachment(RootComponent);
	FirePointComponent->SetRelativeLocation(FVector(67.67f, 0.0f, 0.0f));
}

IPoolableInterface::FOnReturnedToPool& ArglkEnemyCharacter::OnReturnedToPool()
{
	return ReturnToPool;
}

void ArglkEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	FindTarget();
	SetState(EEnemyState::Chasing);

	switch (Type)
	{
	case EEnemyType::Melee:
		RangedWeaponComp->Deactivate();
		StopDistance = 100.f;
		break;
	case EEnemyType::Ranged:
		WeaponComp->Deactivate();
		StopDistance = 1000.f;
		break;
	default:
		break;
	}
}

void ArglkEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateState(DeltaTime);
}

void ArglkEnemyCharacter::FindTarget()
{
	TargetActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ArglkEnemyCharacter::ChaseTarget()
{
	if (!TargetActor) return;

	float Distance = FVector::Dist(GetActorLocation(), TargetActor->GetActorLocation());
	if (Distance > StopDistance)
	{
		FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		AddMovementInput(Direction, 1.0f);
	}
	else {}
}

bool ArglkEnemyCharacter::TimerManager(const FTimerHandle MyTimerHandle) const
{
	FTimerManager& TimerManager = GetWorldTimerManager();

	if (TimerManager.IsTimerActive(MyTimerHandle))
		return true;
	else
		return false;
}


void ArglkEnemyCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
                                    bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
                                    const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (Other)
	{
		FVector PushDir = GetActorLocation() - Other->GetActorLocation();
		PushDir.Z = 0; // Don't push up/down
		PushDir.Normalize();

		if (Other->ActorHasTag("Player"))
		{
			SeparationForce = SeparationForce.GetClampedToMaxSize(2.f);
		}
		else if (Other->ActorHasTag("Enemy"))
		{
			SeparationForce = SeparationForce.GetClampedToMaxSize(2.f);
		}
	}
}

void ArglkEnemyCharacter::Die()
{
	if (bIsDead) return;
	bIsDead = true;
	AttackTimer.Invalidate();
	GetWorld()->GetAuthGameMode<ArglkGameMode>()->SpawnedEnemiesList.Remove(this);
	if (UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
	{
		Pool->ReturnActorToPool(this);
		ReturnToPool.Broadcast(this);
		GetGameInstance()->GetSubsystem<UScoreSubsystem>()->SetScore(1);
		GetWorld()->GetAuthGameMode<ArglkGameMode>()->TryStartWaveTransition();
	}
}

void ArglkEnemyCharacter::Attack()
{
	switch (Type)
	{
	case EEnemyType::Melee:
		WeaponComp->PerformAttack();
		break;
	case EEnemyType::Ranged:
		RangedWeaponComp->PerformAttack();
		break;
	default:
		break;
	}

	AttackTimer.Invalidate();
}

void ArglkEnemyCharacter::OnSpawnFromPool_Implementation()
{
	ApplyBaseStats();
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	bIsDead = false;
	SeparationForce = FVector::ZeroVector;
	FindTarget();

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}


// Refactor to 1 class per state (imma do this when I have time)
// -- Handle States --

void ArglkEnemyCharacter::SetState(EEnemyState NewState)
{
	if (CurrentState == NewState) return;

	const bool bNeedsTick =
		(NewState == EEnemyState::Chasing ||
		 NewState == EEnemyState::Attacking);

	SetActorTickEnabled(bNeedsTick);
	
	ExitState(CurrentState);
	CurrentState = NewState;
	EnterState(CurrentState);
}

void ArglkEnemyCharacter::EnterState(EEnemyState State)
{
	switch (State)
	{
	case EEnemyState::Chasing:

		break;

	case EEnemyState::Attacking:
		if (!WeaponComp)
			SetState(EEnemyState::Chasing);
		break;
	}
}

void ArglkEnemyCharacter::ExitState(EEnemyState State)
{
	switch (State)
	{
	case EEnemyState::Attacking:

		break;

	default:
		break;
	}
}

void ArglkEnemyCharacter::UpdateState(float DeltaTime)
{
	switch (CurrentState)
	{
	case EEnemyState::Chasing:
		UpdateChase(DeltaTime);
		break;

	case EEnemyState::Attacking:
		UpdateAttack(DeltaTime);
		break;
	default:
		break;
	}
}

void ArglkEnemyCharacter::UpdateChase(float DeltaTime)
{
	float Distance = FVector::Dist(GetActorLocation(), TargetActor->GetActorLocation());

	if (TargetActor)
		ChaseTarget();
	else
		FindTarget();

	// seperation logic
	if (!SeparationForce.IsZero())
	{
		AddMovementInput(SeparationForce, 1.0f);
		SeparationForce = FMath::VInterpTo(
			SeparationForce,
			FVector::ZeroVector,
			DeltaTime,
			5.0f);
	}

	if (Distance < StopDistance)
		SetState(EEnemyState::Attacking);
}

void ArglkEnemyCharacter::UpdateAttack(float DeltaTime)
{
	if (TimerManager(AttackTimer)) return;
	GetWorldTimerManager().SetTimer(
		AttackTimer,
		this,
		&ArglkEnemyCharacter::Attack,
		0.7f,
		false
	);

	float StopDistanceSq = StopDistance * StopDistance;
	float DistanceSq = FVector::DistSquared(GetActorLocation(), TargetActor->GetActorLocation());
	if (DistanceSq > StopDistance)
	{
		SetState(EEnemyState::Chasing);
	}
}

float ArglkEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                      class AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.f, MaxHealth);
	if (CurrentHealth <= 0)
	{
		Die();
	}

	return ActualDamage;
}

void ArglkEnemyCharacter::ApplyBaseStats(const TCHAR* DebugString)
{
	Super::ApplyBaseStats(DebugString);
}
