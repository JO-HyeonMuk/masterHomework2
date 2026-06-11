// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWorldSubsystem.h"
#include "TimeWorldSubsystem.h"

bool UMyWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = Cast<UWorld>(Outer);
	return (World && World->GetMapName().Contains(TEXT("StarterMap")));
}

void UMyWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("My Subsystem Begin"));
	UTimeWorldSubsystem* TimeSub = Collection.InitializeDependency<UTimeWorldSubsystem>();
	
	if (TimeSub)
	{
		int32 CurrentHour = TimeSub->GetCurrentHour();
		UE_LOG(LogTemp,Warning, TEXT("Current Time : %d"), CurrentHour);
	}
}

void UMyWorldSubsystem::Deinitialize()
{
	
	UE_LOG(LogTemp, Warning, TEXT("My Subsystem End"));
	
	Super::Deinitialize();
}
