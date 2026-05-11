#include "MyHealthComponent.h"

#include "MyGameState.h"

UMyHealthComponent::UMyHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	MaxHealth = 100.f;
}

void UMyHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
	
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UMyHealthComponent::DamageTake);
}

void UMyHealthComponent::DamageTake(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* Instigator, AActor* Causer)
{
	float FinalDamage = FMath::Min(Damage, CurrentHealth);
	CurrentHealth -= FinalDamage;
	
	OnHealthDamaged.Broadcast(CurrentHealth,MaxHealth,FinalDamage);
	if (CurrentHealth<= 0.f)
	{
		OnHealthDead.Broadcast(Instigator);
		
		if (AMyGameState* GS = GetWorld()->GetGameState<AMyGameState>())
		{
			// "누가(Instigator) 나(GetOwner())를 죽였다!"
			GS->OnMonsterKilled.Broadcast(Instigator, GetOwner());
		}
	}
}

void UMyHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("HP : %f"), CurrentHealth));
}


