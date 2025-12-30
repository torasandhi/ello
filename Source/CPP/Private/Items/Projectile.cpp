#include "Items/Projectile.h"
#include "ObjectPoolSubsystem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"

AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
    SphereCollider->SetupAttachment(RootComponent);
    SphereCollider->SetWorldScale3D(FVector(3.f,3.f,3.f));
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();
    if (SphereCollider)
    {
       SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::DealDamage);
    }
}

void AProjectile::Activate(const FVector& Direction, AActor* InOwner)
{
    SetOwner(InOwner);
    MoveDirection = Direction.GetSafeNormal();

    SetActorHiddenInGame(false);
    SetActorTickEnabled(true);
}

void AProjectile::OnSpawnFromPool_Implementation()
{
    IPoolableInterface::OnSpawnFromPool_Implementation();
    AliveTime = 0.f;
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
       ReturnActorToPool();
       return;
    }

    const FVector Start = GetActorLocation();
    const FVector End = Start + MoveDirection * Speed * DeltaTime;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        Params.AddIgnoredActor(PC->GetPawn());
    }

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
       SetActorLocation(Hit.ImpactPoint);
       
       AActor* HitActor = Hit.GetActor();
       if (HitActor && !HitActor->ActorHasTag("Player"))
       {
           UGameplayStatics::ApplyDamage(
               HitActor,
               BaseDamage,
               nullptr,
               nullptr,
               UDamageType::StaticClass()
           );
       }

       ReturnActorToPool();
       return;
    }

    SetActorLocation(End);
}

void AProjectile::DealDamage(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (OtherActor == this) return;

    if (OtherActor->ActorHasTag("Player")) return;
    
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (OtherActor == PC->GetPawn()) return;
    }

    UGameplayStatics::ApplyDamage(
       OtherActor,
       BaseDamage,
       nullptr,
       nullptr,
       UDamageType::StaticClass()
    );

    ReturnActorToPool();
}

void AProjectile::ReturnActorToPool()
{
    SetActorTickEnabled(false);
    SetActorHiddenInGame(true);

    if (UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
    {
       Pool->ReturnActorToPool(this);
    }
}