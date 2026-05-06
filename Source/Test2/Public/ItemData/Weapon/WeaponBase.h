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

	// 템플릿 메서드 (최종 호출되는 함수, 자식 클래스에서 오버라이드 불가)
	void Fire();

	// 무기 초기화 함수
	void InitializeWeapon(UItemData* InData);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UItemData* WeaponData;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* WeaponMesh;

	// 하위 클래스가 반드시 구현해야 하는 세부 단계 (Primitive Operation)
	virtual void ExecuteShot() PURE_VIRTUAL(AWeaponBase::ExecuteShot, );
	virtual void ApplyRecoil() PURE_VIRTUAL(AWeaponBase::ApplyRecoil, );
};
