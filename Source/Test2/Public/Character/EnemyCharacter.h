// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WorldCollision.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class TEST2_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	void BeginPlay();
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	void OnTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Datum);
	
	void LookForPlayer();

private:
	FTraceDelegate TraceDelegate;
	FTimerHandle TimerHandle;
	FTraceHandle LastTraceHandle;
};
