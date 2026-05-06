#include "ItemData/Weapon/Shotgun.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "ItemData/ItemData.h"

void AShotgun::ExecuteShot()
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return;

	APlayerController* PC = Cast<APlayerController>(OwnerChar->GetController());
	if (!PC) return;

	for (int32 i = 0; i < WeaponData->PelletCount; ++i)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector LaunchDir = CameraRotation.Vector();
		if (WeaponData->SpreadAngle > 0.0f)
		{
			LaunchDir = FMath::VRandCone(LaunchDir, FMath::DegreesToRadians(WeaponData->SpreadAngle));
		}

		FVector End = CameraLocation + (LaunchDir * 5000.0f);

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(OwnerChar);

		if (GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, End, ECC_Visibility, Params))
		{
			DrawDebugLine(GetWorld(), CameraLocation, Hit.ImpactPoint, FColor::Green, false, 1.0f);
		}
	}
}

void AShotgun::ApplyRecoil()
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (OwnerChar && WeaponData)
	{
		// 샷건 반동 적용
		OwnerChar->AddControllerPitchInput(WeaponData->RecoilIntensity * -1.0f);
	}
}