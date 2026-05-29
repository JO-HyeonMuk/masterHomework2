#include "Test2Character.h"
#include "Engine/LocalPlayer.h"
#include "Engine/DamageEvents.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FireDamageType.h"
#include "InputActionValue.h"
#include "MyHealthComponent.h"
#include "Component/QuestComponent.h"
#include "Inventory/InventoryComponent.h"
#include "ItemData/Weapon/WeaponBase.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);


ATest2Character::ATest2Character()
{

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true; 

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComp"));
	
	HealthComp = CreateDefaultSubobject<UMyHealthComponent>(TEXT("HealthComp"));
	
	QuestComp = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComp"));

}

/*float ATest2Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	const UFireDamageType* FireDamage = DamageEvent.DamageTypeClass->GetDefaultObject<UFireDamageType>();

	if (FireDamage)
	{
		ActualDamage *= (1.f + FireDamage->ArmorPeneltration);

		UE_LOG(LogTemp, Warning, TEXT("ByWorld Damage Received"));
	}

	if (EventInstigator)
	{
		UE_LOG(LogTemp, Warning, TEXT("Im Enemy!"));
	}

	return ActualDamage;
}*/


void ATest2Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ATest2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATest2Character::Move);
		
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATest2Character::Look);
		
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ATest2Character::OnFire);
		
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ATest2Character::StartAim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ATest2Character::StopAim);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATest2Character::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATest2Character::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATest2Character::OnFire()
{
	if (InventoryComp && InventoryComp->SpawnedWeaponActor)
	{
		InventoryComp->SpawnedWeaponActor->Fire();
	}
}

void ATest2Character::HandleDeath(AController* InstigatorController)
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

void ATest2Character::StartAim()
{
	bIsAiming = true;
	if (FollowCamera)
	{
		// FOV를 좁혀 줌인 효과 부여
		FollowCamera->SetFieldOfView(AimFOV);
	}
	
	if (InventoryComp && InventoryComp->SpawnedWeaponActor)
	{
		InventoryComp->SpawnedWeaponActor->WeaponData->SpreadAngle = 2.0f; 
	}
}

void ATest2Character::StopAim()
{
	bIsAiming = false;
	if (FollowCamera)
	{
		// 원래 FOV로 복구
		FollowCamera->SetFieldOfView(DefaultFOV);
	}
	
	if (InventoryComp && InventoryComp->SpawnedWeaponActor)
	{
		InventoryComp->SpawnedWeaponActor->WeaponData->SpreadAngle = 
			InventoryComp->SpawnedWeaponActor->DefaultSpreadAngle;
	}
}

void ATest2Character::BeginPlay()
{
	Super::BeginPlay();
	
	if (FollowCamera)
	{
		DefaultFOV = FollowCamera->FieldOfView;
	}

	if (InventoryComp && TestWeaponData)
	{
		InventoryComp->AddItem(TestWeaponData); // 인벤토리에 추가
		InventoryComp->EquipItem(0);           // 0번 슬롯 무기 바로 장착
	}
	
	if (HealthComp)
	{
		HealthComp->OnHealthDead.AddDynamic(this, &ATest2Character::HandleDeath);
	}
}