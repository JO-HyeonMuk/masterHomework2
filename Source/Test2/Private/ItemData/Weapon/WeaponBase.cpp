#include "ItemData/Weapon/WeaponBase.h"
#include "GameFramework/Character.h"
#include "ItemData/ItemData.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponBase::InitializeWeapon(UItemData* InData)
{
	WeaponData = InData;
	if (WeaponData && WeaponData->ItemMesh)
	{
		WeaponMesh->SetStaticMesh(WeaponData->ItemMesh);
		WeaponMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	}
}

void AWeaponBase::Fire()
{
	// 템플릿 메서드 패턴: 뼈대 알고리즘 실행
	if (!WeaponData) return;

	ExecuteShot();
	ApplyRecoil();
}
