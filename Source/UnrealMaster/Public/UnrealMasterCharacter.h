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
	
	virtual float TakeDamage(
		float DamageAmount, 
		struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, 
		AActor* DamageCauser) override;
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
			
	void OnDeath();

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
	
};

