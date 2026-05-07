// BaseGun.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGun.generated.h"

class UGunStatComponent;
class UStaticMeshComponent;
class UArrowComponent;

UCLASS()
class UNREALMASTER_API ABaseGun : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseGun();
	
	UGunStatComponent* GetGunStat() const {return GunStat;}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UGunStatComponent> GunStat;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun")
	TObjectPtr<UStaticMeshComponent> GunMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> FirePoint;
	
	
	UFUNCTION(BlueprintCallable)
	virtual bool Fire();
	
	UFUNCTION(BlueprintNativeEvent)
	bool SandboxFire();							// 위임하는 함수 만들기
												// 구현부 만들면 안 됨
	
	UFUNCTION(BlueprintCallable)
	virtual void Reload();
	
	// 총알 체크
	UFUNCTION(BlueprintCallable)				// 밖에서 조립할 수 있도록 BlueprintCallable로 통일
	bool CheckAmmo();
	
	// 총쏘기
	UFUNCTION(BlueprintCallable)
	void LinetraceOneShot(FVector Direction);
	
	// 사운드
	UFUNCTION(BlueprintCallable)
	void PlaySound(USoundBase* Sound);
	
	// 총알 업데이트 담당
	UFUNCTION(BlueprintCallable)
	void UpdateAmmo();
	
protected:
	bool bCanFire;
	FTimerHandle FireRateCooldownHandle;
	void ResetFireCooldown();
	
	bool bIsReloading;
	FTimerHandle ReloadTimerHandle;
	void OnReloadComplete();
	
};
