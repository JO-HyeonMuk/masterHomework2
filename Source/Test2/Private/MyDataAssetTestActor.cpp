// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDataAssetTestActor.h"
#include "ItemData/MyItemData.h"

// Sets default values
AMyDataAssetTestActor::AMyDataAssetTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyDataAssetTestActor::BeginPlay()
{
	Super::BeginPlay();
	
	StartLoading();
	
}

// Called every frame
void AMyDataAssetTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyDataAssetTestActor::StartLoading()
{
	if (!ItemToLoad.IsValid()) return;
	UAssetManager& AssetManager = UAssetManager::Get();
	
	TArray<FName> Bundles;

	LoadingHandle = AssetManager.LoadPrimaryAsset(ItemToLoad, Bundles,
		FStreamableDelegate::CreateUObject(this, &AMyDataAssetTestActor::OnLoadFinished, ItemToLoad)
	);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("아이템 로드 시작"));
}

void AMyDataAssetTestActor::OnLoadFinished(FPrimaryAssetId LoadedId)
{
	LoadedItem = Cast<UMyItemData>(UAssetManager::Get().GetPrimaryAssetObject(LoadedId));
	if (LoadedItem)
	{
		FString Msg = FString::Printf(TEXT("배달 완료 아이템 이름 : %s"), *LoadedItem->ItemMesh.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Msg);
		// 실제로 메시를 교체... 수치 적용..~~~~~~~~~
	}
}

