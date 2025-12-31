// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseState.h"
#include "StateMachine.generated.h"

class UBaseState;

UCLASS()
class CPP_API UStateMachine : public UObject
{
	GENERATED_BODY()

public:
	void Init(UObject* InOwner)
	{
		Owner = InOwner;
	}

	void ChangeState(TSubclassOf<UBaseState> NewStateClass)
	{
		if (CurrentState)
		{
			CurrentState->OnExit();
		}

		CurrentState = NewObject<UBaseState>(Owner, NewStateClass);
		CurrentState->OnEnter();
	}

	void Update(float DeltaTime)
	{
		if (CurrentState)
		{
			CurrentState->OnUpdate(DeltaTime);
		}
	}

private:
	UPROPERTY()
	UObject* Owner;

	UPROPERTY()
	UBaseState* CurrentState;
};
