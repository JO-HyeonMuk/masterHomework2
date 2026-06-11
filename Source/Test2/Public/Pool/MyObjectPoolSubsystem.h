// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Pool/PooledObjectData.h" 
#include "MyObjectPoolSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPoolerCleanupSignature);

class UPooledObject; 
class AActor;

USTRUCT(BlueprintType)
struct FSingleObjectPool
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Object Pool")
	TArray<TObjectPtr<AActor>> PooledActors;
};

UCLASS()
class TEST2_API UMyObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void InitializePoolData(const TArray<FPooledObjectData>& InData);

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	AActor* GetPooledActor(FString Name);

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void RecycleActor(AActor* PooledActor);

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void Broadcast_PoolerCleanup();
	
	UPROPERTY(BlueprintAssignable, Category = "Object Pool")
	FPoolerCleanupSignature OnPoolerCleanup;

private:
	UPROPERTY()
	TArray<FPooledObjectData> PooledObjectData;

	UPROPERTY()
	TArray<FSingleObjectPool> Pools;
};