// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetInterface.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UWidget;

UCLASS()
class CPP_API UMainMenuWidget : public UUserWidget, public IWidgetInterface
{
	GENERATED_BODY()

public:
	virtual void OnShowWidget_Implementation() override;
	virtual void OnHideWidget_Implementation() override;

private:
	UFUNCTION(BlueprintCallable)
	void OnStartButtonClick();
	UFUNCTION(BlueprintCallable)
	void OnExitButtonClick();
};
