// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "WBP_PlayerHUD.generated.h"

UCLASS()
class TEST2_API UWBP_PlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void UpdateHealthBar(float CurrentHealth, float MaxHealth, float HealthChange);
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
};
