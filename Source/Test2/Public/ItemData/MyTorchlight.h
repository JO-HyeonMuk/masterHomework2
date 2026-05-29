// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyTorchlight.generated.h"

UCLASS()
class TEST2_API AMyTorchlight : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyTorchlight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
public:
	
	UPROPERTY(EditAnywhere)
	TArray<TWeakObjectPtr<AActor>> Items;
	
};
