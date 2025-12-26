#pragma once

#include "CoreMinimal.h"
#include "rglkCharacter.h" 
#include "UPoolableInterface.h"
#include "rglkEnemyCharacter.generated.h"

UCLASS()
class CPP_API ArglkEnemyCharacter : public ArglkCharacter, public IPoolableInterface
{
	GENERATED_BODY()

public:
	ArglkEnemyCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
    
	// Override NotifyHit to keep your "Bumping" logic
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void Die() override;
	virtual void Attack() override;
	
	// Interface Implementation
	virtual void OnSpawnFromPool_Implementation() override;

private:
	// Logic to find player
	void FindTarget();
    
	// Logic to chase
	void ChaseTarget();

private:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, Category = "AI")
	float StopDistance = 100.0f; // Increased slightly for stability

	// Used for the separation logic
	FVector SeparationForce;
};