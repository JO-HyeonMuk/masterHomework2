// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Cloth.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

void AItem_Cloth::OnFireDetected_Implementation(float Temperature, FVector HitLocation)
{
	if (FireEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			FireEffect,
			GetActorLocation(),
			GetActorRotation(),
			FVector(1.f)
		);
	}
}
