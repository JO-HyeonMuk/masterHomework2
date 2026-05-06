#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Shotgun.generated.h"

UCLASS()
class TEST2_API AShotgun : public AWeaponBase
{
	GENERATED_BODY()
    
protected:
	virtual void ExecuteShot() override;
	virtual void ApplyRecoil() override;
};