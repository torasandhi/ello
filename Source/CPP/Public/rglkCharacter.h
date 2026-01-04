#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "rglkCharacter.generated.h"

class USceneComponent;
class UWeaponComponent;
struct FDataTableRowHandle;

UCLASS()
class CPP_API ArglkCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ArglkCharacter();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentHealth = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	float BaseDamage = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MoveSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
	bool bIsDead = false;

	virtual void ApplyBaseStats(const TCHAR* DebugString = TEXT("ApplyBaseStats"));

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWeaponComponent* WeaponComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* FirePointComponent;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Attack();
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Die();
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void PlayAttackEffects();
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	FDataTableRowHandle CharacterStats;
};
