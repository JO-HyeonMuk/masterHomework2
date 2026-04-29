// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	TraceDelegate.BindUObject(this, &AEnemyCharacter::OnTraceCompleted);
	
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyCharacter::LookForPlayer, 0.5f, true);
}


void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::LookForPlayer()
{
	FVector Start = GetActorLocation();
	FVector End = Start + (GetActorForwardVector() * 1000.0f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	LastTraceHandle = GetWorld()->AsyncLineTraceByChannel(
		EAsyncTraceType::Single,
		Start,
		End,
		ECC_Pawn,
		Params,
		FCollisionResponseParams::DefaultResponseParam,
		&TraceDelegate
	);
}

void AEnemyCharacter::OnTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Datum)
{
	if (Datum.OutHits.Num() > 0)
	{
		AActor* HitActor = Datum.OutHits[0].GetActor();
		if (HitActor && HitActor->ActorHasTag(TEXT("Player")))
		{
			UE_LOG(LogTemp, Warning, TEXT("플레이어 발견!"));
		}
	}
}