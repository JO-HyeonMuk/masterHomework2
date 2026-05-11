#include "Component/QuestComponent.h"

#include "MyGameState.h"

void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (AMyGameState* GS = GetWorld()->GetGameState<AMyGameState>())
	{
		GS->OnMonsterKilled.AddDynamic(this, &UQuestComponent::HandleMonsterKilled);
	}
}

void UQuestComponent::HandleMonsterKilled(AController* Killer, AActor* Victim)
{
	
	KillCount++;
	
	UE_LOG(LogTemp, Warning, TEXT("퀘스트 진행 중... 현재: %d / %d"), KillCount, TargetKillCount);

	if (KillCount >= TargetKillCount)
	{
		UE_LOG(LogTemp, Error, TEXT("퀘스트 완료! 100마리를 처치했습니다."));
	}
}

