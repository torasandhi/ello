#pragma once

#include "CoreMinimal.h"
#include "rglkCharacter.h" // Inheriting from your Base Character
#include "InputActionValue.h"
#include "rglkPlayerCharacter.generated.h"

// Forward declarations to reduce compile time
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class URangedWeaponComponent;

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Melee,
	Ranged,

	COUNT
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);

UCLASS()
class CPP_API ArglkPlayerCharacter : public ArglkCharacter
{
	GENERATED_BODY()

public:
	ArglkPlayerCharacter();

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	float GetHealthPercent() const;

protected:
	virtual void BeginPlay() override;
	virtual void Attack() override;
	virtual void Die() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;
public:
	/** Camera Boom: Positions the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	/** Follow Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	URangedWeaponComponent* RangedComp;

	/** Actions */
	void Execute_Move(const FInputActionValue& Value);
	void Execute_Attack(const FInputActionValue& Value);
	void Execute_Swap(const FInputActionValue& Value);
	
private:
	EAttackType AttackType = EAttackType::Ranged;
	int32 TypeCount = static_cast<int>(EAttackType::COUNT);
	int32 CurrentTypeIndex = 0;
};

