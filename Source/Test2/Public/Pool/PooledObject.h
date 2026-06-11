// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PooledObject.generated.h"

class UMyObjectPoolSubsystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEST2_API UPooledObject : public UActorComponent
{
	GENERATED_BODY()

public:
	void Init(UMyObjectPoolSubsystem* Owner);
	
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void RecycleSelf();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Object Pool")
	bool bIsPoolActive;

private:
	UPROPERTY()
	TObjectPtr<class UMyObjectPoolSubsystem> ObjectPool;
    
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
};