#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UItemData;

UCLASS()
class TEST2_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();
	void BeginPlay();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UItemData* WeaponData;
	
	void InitializeWeapon(class UItemData* InData);
	
	virtual void Fire();
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* WeaponMesh;
};
