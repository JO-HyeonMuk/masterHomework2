// Fill out your copyright notice in the Description page of Project Settings.

#include "Pool/PooledObject.h"
#include "Pool/MyObjectPoolSubsystem.h" 
#include "GameFramework/Actor.h"

void UPooledObject::Init(UMyObjectPoolSubsystem* Owner)
{
	ObjectPool = Owner;
	bIsPoolActive = false;
}

void UPooledObject::RecycleSelf()
{
	AActor* OwnerActor = GetOwner();
	
	if (OwnerActor && ObjectPool)
	{
		ObjectPool->RecycleActor(OwnerActor);
	}
}

void UPooledObject::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}