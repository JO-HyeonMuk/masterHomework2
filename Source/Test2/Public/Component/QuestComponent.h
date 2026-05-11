// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEST2_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	// 게시판 소식을 들었을 때 실행할 함수
	UFUNCTION()
	void HandleMonsterKilled(AController* Killer, AActor* Victim);

	int32 KillCount = 0;
	const int32 TargetKillCount = 100;
};
