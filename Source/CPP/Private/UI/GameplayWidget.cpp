// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameplayWidget.h"
#include "CPP/CPP.h"
#include "Player/rglkPlayerCharacter.h"
#include "Subsystem/Instance/ScoreSubsystem.h"

void UGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APawn* OwningPawn = GetOwningPlayerPawn();
	if (ArglkPlayerCharacter* MyChar = Cast<ArglkPlayerCharacter>(OwningPawn))
	{
		BindToPlayer(MyChar);
	}
	else
	{
		PRINT_DEBUG_MESSAGE("GameplayWidget: Could not find ArglkPlayerCharacter!");
	}
}

void UGameplayWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	UScoreSubsystem* SS = GetGameInstance()->GetSubsystem<UScoreSubsystem>();
	SS->OnScoreChanged.RemoveAll(this);		
}

void UGameplayWidget::BindToPlayer(ArglkPlayerCharacter* Player)
{
	if (!Player) return;
	Player->OnHealthChanged.AddDynamic(
		this, &UGameplayWidget::OnHealthChanged
	);
	OnHealthChanged(Player->GetHealthPercent() * Player->MaxHealth, Player->MaxHealth);

	UScoreSubsystem* ScoreSubsystem =GetGameInstance()->GetSubsystem<UScoreSubsystem>();
	ScoreSubsystem->OnScoreChanged.AddUObject(
		this, &UGameplayWidget::OnScoreChanged);
}

void UGameplayWidget::OnHealthChanged(float Current, float Max)
{
	HealthBar->SetPercent(Current / Max);
}

void UGameplayWidget::OnScoreChanged(int32 NewScore) const
{
	ScoreText->SetText(FText::AsNumber(NewScore));
}
