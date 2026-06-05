// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "UnrealMasterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ABaseGun;
class UMyActorComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AUnrealMasterCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
public:
	AUnrealMasterCharacter();
	
	virtual void Tick(float DeltaSecond) override;
	
	UFUNCTION(BlueprintPure)
	UMyActorComponent* GetHealthComponent() const {return HealthComponent;}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UMyActorComponent> HealthComponent;
	
protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void Move(const FInputActionValue& value); 
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void StartFire(const FInputActionValue& value);
	UFUNCTION()
	void StopFire(const FInputActionValue& value);
	UFUNCTION()
	void Reload(const FInputActionValue& value);
	UFUNCTION()
	void StartAim(const FInputActionValue& value);
	UFUNCTION()
	void StopAim(const FInputActionValue& value);
	UFUNCTION()
	void OnDeath(AController* DeathInstigator);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	TSubclassOf<ABaseGun> GunClassToSpawn;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun")
	TObjectPtr<ABaseGun> CurrentGun;
	
	UFUNCTION()
	void SpawnSelectedGun();

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
private:
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;
	
	void ApplyRecoil(float Pitch, float Yaw);
	
	float RecoilPitchRemaining;  // 남은 반동량
	float RecoilYawRemaining; 
	float RecoilRecoverySpeed;   // 복귀 속도
	bool bIsRecoiling; 
	
	float DefaultArmLength;		// 기본 카메라 거리
	float AimArmLength;			// 조준 시 카메라 거리
	float AimInterpSpeed;		// 보간 속도
	bool bIsAiming;				// 조준 중 여부
	float DefaultFOV;			// 기본 카메라 시야각
	float AimFOV;				// 조준 시 카메라 시야각
	
};

