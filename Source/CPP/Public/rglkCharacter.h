#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "rglkCharacter.generated.h"

class UWeaponComponent;

UCLASS()
class CPP_API ArglkCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ArglkCharacter();

protected:
	virtual void BeginPlay() override;

public: 
	virtual void Tick(float DeltaTime) override;

	// --- SHARED STATS --- //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MoveSpeed = 600.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UWeaponComponent* WeaponComp;
	// --- SHARED ACTIONS --- //
	// Both AI and Player need to be able to attack, 
	// but the TRIGGER (Input vs AI Decision) is different.
    
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Attack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Die();
    
	// We do NOT put SetupPlayerInputComponent here, 
	// because the AI doesn't have a PlayerController!
};