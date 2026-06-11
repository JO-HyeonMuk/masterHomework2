// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/AssetManager.h"
#include "MyDataAssetTestActor.generated.h"

UCLASS()
class TEST2_API AMyDataAssetTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyDataAssetTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = "ItemTest")
	FPrimaryAssetId ItemToLoad;
	
	UPROPERTY(EditAnywhere, Category = "ItemTest")
	class UMyItemData* LoadedItem;
	
	void StartLoading();
	
	TSharedPtr<FStreamableHandle> LoadingHandle;
	
	void OnLoadFinished(FPrimaryAssetId LoadedId);

};
