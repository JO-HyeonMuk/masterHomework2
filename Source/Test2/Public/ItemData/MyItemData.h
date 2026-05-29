// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyItemData.generated.h"

UCLASS()
class TEST2_API UMyItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	//에셋 매니저가 이 파일을 식별할수 있도록 넘겨주는 ID예요
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(ItemType, GetFName());
	}
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FPrimaryAssetType ItemType;
	//Struct처럼 활용!!!
	UPROPERTY(EditAnywhere, Category = "Visual", meta = (AssetBundles = "Mesh"))
	TSoftObjectPtr<USkeletalMesh> ItemMesh;
	UPROPERTY(EditAnywhere, Category = "Visual", meta = (AssetBundles = "Character"))
	TSoftObjectPtr<USkeletalMesh> ItemMesh2;
	
	UPROPERTY(EditAnywhere, Category= "Stats")
	float AttackPower;
};
