// UnrealMasterCharacter.cpp

#include "../Public/UnrealMasterCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/DamageEvents.h"
#include "FireDamageType.h"
#include "MasterPlayerController.h"
#include "BaseGun.h"
#include "GunStatComponent.h"
#include "MyActorComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AUnrealMasterCharacter::AUnrealMasterCharacter()
{
	
	NormalSpeed = 500.0f;
	SprintSpeedMultiplier = 2.f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	
	RecoilPitchRemaining = 0.f;
	RecoilYawRemaining = 0.f;
	RecoilRecoverySpeed = 15.f; 
	bIsRecoiling = false;
	
	DefaultArmLength = 200.0f;
	AimArmLength = 50.f;
	AimInterpSpeed = 5.f;
	bIsAiming = false;
	DefaultFOV = 90.f;
	AimFOV = 60.f;
	
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
	CameraBoom->TargetArmLength = DefaultArmLength;
	CameraBoom->bUsePawnControlRotation = true; 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false; 
	
	HealthComponent = CreateDefaultSubobject<UMyActorComponent>(TEXT("HealthComponent"));
}

void AUnrealMasterCharacter::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);
	
	// Aim 관련 보간
	float CurrentArmLength = CameraBoom->TargetArmLength;
	float CurrentFOV = FollowCamera->FieldOfView;

	float TargetArmLength = bIsAiming? AimArmLength : DefaultArmLength;
	float TargetFOV = bIsAiming? AimFOV : DefaultFOV;
	CameraBoom->TargetArmLength = FMath::FInterpTo(
		CurrentArmLength,
		TargetArmLength,
		DeltaSecond,
		AimInterpSpeed
	);
	FollowCamera->FieldOfView = FMath::FInterpTo(
		CurrentFOV,
		TargetFOV,
		DeltaSecond,
		AimInterpSpeed
	);
	
	
	// 카메라 피드백 관련 보간
	if (bIsRecoiling)
	{
		float NewPitchRemaining = FMath::FInterpTo(RecoilPitchRemaining, 0.f, DeltaSecond, RecoilRecoverySpeed);
		float NewYawRemaining = FMath::FInterpTo(RecoilYawRemaining, 0.f, DeltaSecond, RecoilRecoverySpeed);
	
		AddControllerPitchInput(-RecoilPitchRemaining + NewPitchRemaining);
		AddControllerPitchInput(-RecoilYawRemaining + NewYawRemaining);
	
		RecoilPitchRemaining = NewPitchRemaining;
		RecoilYawRemaining = NewYawRemaining;
	
		if (FMath::IsNearlyZero(RecoilPitchRemaining) && FMath::IsNearlyZero(RecoilYawRemaining))
		{
			bIsRecoiling = false;
		}	
	}
}

/*float AUnrealMasterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	const UFireDamageType* FireDamage = DamageEvent.DamageTypeClass->GetDefaultObject<UFireDamageType>();
	// 들어온 데미지 타입이 UFireDamageType인지 확인하며 CDO 가져오기
	// 다른 타입이면 nullptr 반환 → if문에서 걸러짐

	if (FireDamage)
	{
		// 화상 - 불 데미지일 경우 방어구 관통 수치만큼 데미지 증폭
		ActualDamage *= (1.f + FireDamage->ArmorPenetration);
		
		// 화상 이펙트, 사운드 등
		UE_LOG(LogTemp, Warning, TEXT("Fire Damage Received!"));
	}

	//CurrentHP -= ActualDamage; //변수 없음
	return ActualDamage;
}*/

void AUnrealMasterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (HealthComponent)
	{
		HealthComponent->OnHealthDead.AddUniqueDynamic(this, &AUnrealMasterCharacter::OnDeath);
	}
	
	SpawnSelectedGun();
}


void AUnrealMasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		if (AMasterPlayerController* PlayerController = Cast<AMasterPlayerController>(GetController()))
		{	
			if (PlayerController -> MoveAction) 
			{
				EnhancedInputComponent->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,	
					this,			
					&AUnrealMasterCharacter::Move		
					);
			}
			if (PlayerController -> JumpAction)
			{
				EnhancedInputComponent->BindAction( 
					PlayerController->JumpAction,	
					ETriggerEvent::Triggered,	
					this,							
					&AUnrealMasterCharacter::StartJump	
					);
				EnhancedInputComponent->BindAction( 
					PlayerController->JumpAction,	 
					ETriggerEvent::Completed,
					this,							
					&AUnrealMasterCharacter::StopJump	
					);
			}
			if (PlayerController -> LookAction)
			{
				EnhancedInputComponent->BindAction( 
					PlayerController->LookAction,	
					ETriggerEvent::Triggered,	
					this,							
					&AUnrealMasterCharacter::Look	
					);
			}
			if (PlayerController -> SprintAction)
			{
				EnhancedInputComponent->BindAction( 
					PlayerController->SprintAction,	
					ETriggerEvent::Triggered,	
					this,							
					&AUnrealMasterCharacter::StartSprint	
					);
				EnhancedInputComponent->BindAction( 
					PlayerController->SprintAction,	 
					ETriggerEvent::Completed,
					this,							
					&AUnrealMasterCharacter::StopSprint	
					);
			}
			if (PlayerController -> FireAction)
			{
				EnhancedInputComponent->BindAction( 
					PlayerController->FireAction,	
					ETriggerEvent::Triggered,	
					this,							
					&AUnrealMasterCharacter::StartFire	
					);
				EnhancedInputComponent->BindAction( 
					PlayerController->FireAction,	 
					ETriggerEvent::Completed,
					this,							
					&AUnrealMasterCharacter::StopFire	
					);
			}
			if (PlayerController -> ReloadAction)
			{
				EnhancedInputComponent->BindAction( 
					PlayerController->ReloadAction,	
					ETriggerEvent::Triggered,	
					this,							
					&AUnrealMasterCharacter::Reload	
					);
			}
			if (PlayerController -> AimAction)
			{
				EnhancedInputComponent->BindAction( 
					PlayerController->AimAction,	
					ETriggerEvent::Triggered,	
					this,							
					&AUnrealMasterCharacter::StartAim	
					);
				EnhancedInputComponent->BindAction( 
					PlayerController->AimAction,	 
					ETriggerEvent::Completed,
					this,							
					&AUnrealMasterCharacter::StopAim	
					);
			}
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AUnrealMasterCharacter::SpawnSelectedGun()
{
	UWorld* World = GetWorld();
	if (!World) return;
	if (!GunClassToSpawn) return;
	
	CurrentGun = World->SpawnActor<ABaseGun>(GunClassToSpawn);
	
	CurrentGun->AttachToComponent(
		GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		TEXT("GunSocket")
	); // hand_r 소켓에 붙여서, 손을 따라다니도록
	
}

void AUnrealMasterCharacter::ApplyRecoil(float Pitch, float Yaw)
{
	bIsRecoiling = true;
	AddControllerPitchInput(Pitch);
	AddControllerYawInput(Yaw);
	
	RecoilPitchRemaining = Pitch;
	RecoilYawRemaining = Yaw;
}

void AUnrealMasterCharacter::Move(const FInputActionValue& value)
{
	FVector2D MovementVector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		if (!FMath::IsNearlyZero(MovementVector.X)) 
		{
			AddMovementInput(ForwardDirection, MovementVector.X); 
		}
		if (!FMath::IsNearlyZero(MovementVector.Y)) 
		{
			AddMovementInput(RightDirection, MovementVector.Y);
		}
	}
}

void AUnrealMasterCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump(); 
	}
}

void AUnrealMasterCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void AUnrealMasterCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookAxisVector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AUnrealMasterCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AUnrealMasterCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void AUnrealMasterCharacter::StartFire(const FInputActionValue& value)
{
	if (CurrentGun)
	{
		if (CurrentGun->Fire())
		{
			ApplyRecoil(CurrentGun->GetGunStat()->RecoilPitch, CurrentGun->GetGunStat()->RecoilYaw);
		}
	}

}

void AUnrealMasterCharacter::StopFire(const FInputActionValue& value)
{
}

void AUnrealMasterCharacter::Reload(const FInputActionValue& value)
{
	if (CurrentGun)
	{
		CurrentGun->Reload();
	}
}

void AUnrealMasterCharacter::StartAim(const FInputActionValue& value)
{
	bIsAiming = true;
}

void AUnrealMasterCharacter::StopAim(const FInputActionValue& value)
{
	bIsAiming = false;
}

void AUnrealMasterCharacter::OnDeath(AController* DeathInstigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Player Dead"));
	
	// 캐릭터 이동 기능 비활성화
	GetCharacterMovement()->DisableMovement();
	
	// 살아있을 때는 Capsule이 충돌을 담당, 레그돌 상태에서는 Mesh가 충돌을 담당
	// 캡슐 충돌 비활성화
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 레그돌용 충돌 프로파일로 변경
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	// Skeletal Mesh의 물리 시뮬레이션 활성화
	GetMesh()->SetSimulatePhysics(true);
	
}

