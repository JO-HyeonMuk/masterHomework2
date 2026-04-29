#include "Inventory/InventoryComponent.h"
#include "GameFramework/Character.h"
#include "ItemData/Weapon/WeaponBase.h"

UInventoryComponent::UInventoryComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

	Items.Reserve(20);
}

void UInventoryComponent::AddItem(UItemData* NewItem)
{
	if (NewItem)
	{
		Items.Add(NewItem);
		
		UE_LOG(LogTemp, Warning, TEXT("아이템 획득: %s"), *NewItem->ItemName.ToString());
		
	}
}

void UInventoryComponent::EquipItem(int32 SlotIndex)
{
	if (!Items.IsValidIndex(SlotIndex) || Items[SlotIndex]->ItemType != EItemType::Weapon) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;
	
	if (SpawnedWeaponActor)
	{
		SpawnedWeaponActor->Destroy();
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.Instigator = OwnerCharacter;
	
	AWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(AWeaponBase::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	SpawnedWeaponActor = NewWeapon;
	
	if (NewWeapon)
	{
		NewWeapon->InitializeWeapon(Items[SlotIndex]);
    
		NewWeapon->AttachToComponent(OwnerCharacter->GetMesh(), 
			FAttachmentTransformRules::SnapToTargetIncludingScale, 
			TEXT("hand_r"));
	}
}


