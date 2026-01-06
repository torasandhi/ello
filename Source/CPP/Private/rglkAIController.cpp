// Fill out your copyright notice in the Description page of Project Settings.


#include "rglkAIController.h"
#include "Enemy/rglkEnemyCharacter.h"

void ArglkAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ArglkEnemyCharacter* EnemyAI = Cast<ArglkEnemyCharacter>(InPawn))
	{
		if (!EnemyAI->BehaviorTree) return;
		RunBehaviorTree(EnemyAI->BehaviorTree);
	}
}
