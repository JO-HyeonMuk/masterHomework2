// Fill out your copyright notice in the Description page of Project Settings.

#include "Pool/MyObjectPoolSubsystem.h"
#include "Pool/PooledObject.h"
#include "Pool/PooledObjectData.h"
#include "Engine/World.h"
#include "Engine/Engine.h" 

void UMyObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UMyObjectPoolSubsystem::Deinitialize()
{
    Broadcast_PoolerCleanup();
    
    for (auto& SinglePool : Pools)
    {
        for (auto& ActorPtr : SinglePool.PooledActors)
        {
            if (ActorPtr)
            {
                ActorPtr->Destroy();
            }
        }
    }
    Pools.Empty();
    Super::Deinitialize();
}

void UMyObjectPoolSubsystem::InitializePoolData(const TArray<FPooledObjectData>& InData)
{
    PooledObjectData = InData;
    Pools.Empty();

    UWorld* World = GetWorld();
    if (!World) return;

    for (int32 PoolIndex = 0; PoolIndex < PooledObjectData.Num(); PoolIndex++)
    {
        FSingleObjectPool TargetPool;
       
        for (int32 ObjectIndex = 0; ObjectIndex < PooledObjectData[PoolIndex].PoolSize; ObjectIndex++)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Name = FName(FString::Printf(TEXT("%s_%d_%d"), *PooledObjectData[PoolIndex].ActorName, PoolIndex, ObjectIndex));
            SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            
            AActor* SpawnedActor = World->SpawnActor(PooledObjectData[PoolIndex].ActorTemplate, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
            if (!SpawnedActor) continue;

#if WITH_EDITOR
            SpawnedActor->SetActorLabel(FString::Printf(TEXT("%s_%d"), *PooledObjectData[PoolIndex].ActorName, ObjectIndex));
#endif
          
            SpawnedActor->SetActorEnableCollision(false); 
            SpawnedActor->SetActorHiddenInGame(true);      
            SpawnedActor->SetActorTickEnabled(false);     

            TArray<UPrimitiveComponent*> PrimitiveComps;
            SpawnedActor->GetComponents<UPrimitiveComponent>(PrimitiveComps);
            for (UPrimitiveComponent* Comp : PrimitiveComps)
            {
                if (Comp)
                {
                    Comp->SetSimulatePhysics(false);          
                    Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
                    Comp->PutRigidBodyToSleep();              
                }
            }

            UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);
            if (PoolComp)
            {
                PoolComp->RegisterComponent();
                SpawnedActor->AddInstanceComponent(PoolComp);
                PoolComp->Init(this);
                PoolComp->bIsPoolActive = false; 
            }
          
            TargetPool.PooledActors.Add(SpawnedActor);
        }
       
        Pools.Add(TargetPool);
    }
}

AActor* UMyObjectPoolSubsystem::GetPooledActor(FString Name)
{
    int32 CurrentPoolIndex = -1;
    for (int32 i = 0; i < Pools.Num(); i++)
    {
        if (PooledObjectData[i].ActorName == Name)
        {
            CurrentPoolIndex = i;
            break;
        }
    }
    if (CurrentPoolIndex == -1) return nullptr;

    for (AActor* Actor : Pools[CurrentPoolIndex].PooledActors)
    {
        if (IsValid(Actor)) 
        {
            if (!Actor->ActorHasTag(FName("Active")))
            {
                Actor->Tags.Add(FName("Active"));

                // 꺼내 가기 직전 락 완전 해제 프로토콜
                Actor->SetActorEnableCollision(false);
                TArray<UPrimitiveComponent*> PrimitiveComps;
                Actor->GetComponents<UPrimitiveComponent>(PrimitiveComps);
                for (UPrimitiveComponent* Comp : PrimitiveComps)
                {
                    if (Comp) Comp->SetSimulatePhysics(false);
                }

                Actor->SetActorHiddenInGame(false);
                Actor->SetActorTickEnabled(true);
                
                // 💡 꺼내올 때 오토 클린업 바인딩 추가
                if (UPooledObject* PoolComp = Cast<UPooledObject>(Actor->GetComponentByClass(UPooledObject::StaticClass())))
                {
                    PoolComp->bIsPoolActive = true;
                    OnPoolerCleanup.AddUniqueDynamic(PoolComp, &UPooledObject::RecycleSelf);
                }
                
                return Actor; 
            }
        }
    }
    
    UWorld* World = GetWorld();
    if (!World) return nullptr;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = FName(FString::Printf(TEXT("%s_Dynamic_%d"), *PooledObjectData[CurrentPoolIndex].ActorName, Pools[CurrentPoolIndex].PooledActors.Num()));
    SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    AActor* SpawnedActor = World->SpawnActor(PooledObjectData[CurrentPoolIndex].ActorTemplate, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
    if (!SpawnedActor) return nullptr;

#if WITH_EDITOR
    SpawnedActor->SetActorLabel(SpawnedActor->GetName());
#endif
    
    SpawnedActor->Tags.Add(FName("Active"));
    Pools[CurrentPoolIndex].PooledActors.Add(SpawnedActor);
    
    SpawnedActor->SetActorHiddenInGame(false);
    SpawnedActor->SetActorEnableCollision(false);
    SpawnedActor->SetActorTickEnabled(true);
    
    if (UPooledObject* PoolComp = Cast<UPooledObject>(SpawnedActor->GetComponentByClass(UPooledObject::StaticClass())))
    {
        PoolComp->bIsPoolActive = true;
        OnPoolerCleanup.AddUniqueDynamic(PoolComp, &UPooledObject::RecycleSelf);
    }
    
    return SpawnedActor;
}

void UMyObjectPoolSubsystem::RecycleActor(AActor* PooledActor)
{
    if (!IsValid(PooledActor)) return;
    
    PooledActor->Tags.Remove(FName("Active"));
    
    PooledActor->SetActorHiddenInGame(true);
    PooledActor->SetActorEnableCollision(false);
    PooledActor->SetActorTickEnabled(false);
    PooledActor->SetActorLocation(FVector::ZeroVector);
    
    TArray<UPrimitiveComponent*> PrimitiveComps;
    PooledActor->GetComponents<UPrimitiveComponent>(PrimitiveComps);
    for (UPrimitiveComponent* Comp : PrimitiveComps)
    {
        if (Comp)
        {
            Comp->SetSimulatePhysics(false);
            Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            Comp->PutRigidBodyToSleep();
        }
    }

    if (UPooledObject* PoolCompRef = Cast<UPooledObject>(PooledActor->GetComponentByClass(UPooledObject::StaticClass())))
    {
        OnPoolerCleanup.RemoveDynamic(PoolCompRef, &UPooledObject::RecycleSelf);
        PoolCompRef->bIsPoolActive = false;
    }
}

void UMyObjectPoolSubsystem::Broadcast_PoolerCleanup()
{
    OnPoolerCleanup.Broadcast();
}

