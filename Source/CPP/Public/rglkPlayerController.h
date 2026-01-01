// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "rglkPlayerController.generated.h"

class UMainWidget;
class UInputMappingContext;
class UInputAction;

UENUM(BlueprintType)
enum class EInputState : uint8
{
	Gameplay,
	UI
};

UCLASS()
class CPP_API ArglkPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	void SetInputState(EInputState NewState);


	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> UI_Main;
	UPROPERTY(EditDefaultsOnly, Category="UI")
	UMainWidget* MainWidget;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* GameplayContext;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* UIContext;
	UPROPERTY(EditDefaultsOnly, Category="Actions")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category="Actions")
	UInputAction* StickAimAction;
	UPROPERTY(EditDefaultsOnly, Category="Actions")
	UInputAction* AttackAction;
	UPROPERTY(EditDefaultsOnly, Category="Actions")
	UInputAction* SwapUtilAction;
	UPROPERTY(EditDefaultsOnly, Category="Actions")
	UInputAction* CheckMouseAction;	

private:
	EInputState CurrentState = EInputState::Gameplay;
	bool bUseMouseAim = false;
	/**OnActions*/
	void OnAttackAction(const FInputActionValue& value);
	void OnMoveAction(const FInputActionValue& value);
	void OnSwapUtilAction(const FInputActionValue& value);
	void OnStickAimAction(const FInputActionValue& value);
	void OnMouseMove(const FInputActionValue& value);
	void OnUpdateMouseAim();
};
