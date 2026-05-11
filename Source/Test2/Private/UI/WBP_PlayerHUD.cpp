
#include "UI/WBP_PlayerHUD.h"
#include "MyHealthComponent.h"


void UWBP_PlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	APawn* OwningPawn = GetOwningPlayerPawn();
	if (OwningPawn)
	{
		UMyHealthComponent* HealthComp = OwningPawn->FindComponentByClass<UMyHealthComponent>();
		if (HealthComp)
		{
			HealthComp -> OnHealthDamaged.AddDynamic(this, &UWBP_PlayerHUD::UpdateHealthBar);
		}
	}
}

void UWBP_PlayerHUD::UpdateHealthBar(float CurrentHealth, float MaxHealth, float HealthChange)
{
	if (HealthBar)
	{
		float TargetPercent = CurrentHealth / MaxHealth;
		
		HealthBar->SetPercent(TargetPercent);
	}
}
