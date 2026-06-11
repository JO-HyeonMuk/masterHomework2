// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBox.h"
#include "MyWeaponStruct.h"
#include "Engine/AssetManager.h"

AWeaponBox::AWeaponBox()
{

}
void AWeaponBox::BeginPlay()

{
	Super::BeginPlay();
	OpenBox();
}

void AWeaponBox::OpenBox()
{
	if (!WeaponTable) return;
	
	TArray<FWeaponData*> AllWeapons;
	WeaponTable->GetAllRows<FWeaponData>(TEXT(""), AllWeapons);
	
	if (AllWeapons.Num() == 0) return;
	FWeaponData* SelectedWeapon = AllWeapons[FMath::RandRange(0, AllWeapons.Num() - 1)];
	
	if (SelectedWeapon && !SelectedWeapon->WeaponClass.IsNull())
	{
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		
		TSoftClassPtr<AActor> WeaponSoftClass = SelectedWeapon->WeaponClass;
		FString WeaponName = SelectedWeapon->WeaponName;

		StreamingHandle = Streamable.RequestAsyncLoad(WeaponSoftClass.ToSoftObjectPath(), [this, WeaponSoftClass, WeaponName]()
		{
			if (WeaponSoftClass.IsValid())
			{
				UClass* LoadedClass = WeaponSoftClass.Get();
				if (LoadedClass)
				{
					FActorSpawnParameters SpawnParams;
					GetWorld()->SpawnActor<AActor>(LoadedClass, GetActorLocation() + FVector(0, 0, 100), FRotator::ZeroRotator, SpawnParams);
                    
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("비동기 소환 완료 : %s"), *WeaponName));
				}
			}
		});
	}
}
