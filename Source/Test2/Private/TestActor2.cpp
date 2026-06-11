// Fill out your copyright notice in the Description page of Project Settings.
#include "TestActor2.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"


// Sets default values
ATestActor2::ATestActor2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActor2::BeginPlay()
{
	Super::BeginPlay();
	LoadWithSoftPtr();
	
}

// Called every frame
void ATestActor2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestActor2::LoadWithSoftPtr()
{
	
	if (MySoftMesh.IsNull()) return;
	if (MySoftMesh.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("이미 로드됨, 메모리에서 즉시 참조하겠습니다."));
		MyOnLoadCompleted();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("메모리에 없음, 처음부터 로드를 실행합니다"));
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		Streamable.RequestAsyncLoad(MySoftMesh.ToSoftObjectPath(),FStreamableDelegate::CreateUObject(this, &ATestActor2::MyOnLoadCompleted));
	}

}

void ATestActor2::MyOnLoadCompleted()
{
	UStaticMesh* FinalMesh = MySoftMesh.Get();
	if (FinalMesh)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("성공 : %s"), *FinalMesh->GetName()));
	}
	
}