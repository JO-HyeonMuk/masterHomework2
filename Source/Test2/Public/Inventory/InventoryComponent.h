#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData/ItemData.h"
#include "InventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEST2_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(UItemData* NewItem);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void EquipItem(int32 SlotIndex);
	
	
	UPROPERTY()
	class AWeaponBase* SpawnedWeaponActor;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<UItemData*> Items;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UItemData* CurrentEquippedWeapon;
};
