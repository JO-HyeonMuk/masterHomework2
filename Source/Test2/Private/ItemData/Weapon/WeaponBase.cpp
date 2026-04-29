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

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (WeaponData && WeaponData->ItemMesh)
	{
		WeaponMesh->SetStaticMesh(WeaponData->ItemMesh);
		
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeaponBase::InitializeWeapon(UItemData* InData)
{
	WeaponData = InData;
	if (WeaponData && WeaponMesh && WeaponData->ItemMesh)
	{
		WeaponMesh->SetStaticMesh(WeaponData->ItemMesh);
		
		WeaponMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
        
		UE_LOG(LogTemp, Warning, TEXT("메쉬 설정 완료: %s"), *WeaponData->ItemMesh->GetName());
	}
}

void AWeaponBase::Fire()
{
	if (!WeaponData) return;

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
	
	OwnerChar->AddControllerPitchInput(WeaponData->RecoilIntensity * -1.0f);
}

