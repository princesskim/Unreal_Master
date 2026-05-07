// BaseGun.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGun.generated.h"

class UGunStatComponent;
class UStaticMeshComponent;

UCLASS()
class UNREALMASTER_API ABaseGun : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseGun();
	
	UGunStatComponent* GetGunStat() const {return GunStat;}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGunStatComponent* GunStat;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun")
	UStaticMeshComponent* GunMesh;
	
	UFUNCTION(BlueprintCallable)
	virtual bool Fire();
	
	void FireTrace(FVector End);
	
	UFUNCTION(BlueprintCallable)
	virtual void Reload();
	
private:
	bool bCanFire;
	FTimerHandle FireRateCooldownHandle;
	void ResetFireCooldown();
	
	bool bIsReloading;
	FTimerHandle ReloadTimerHandle;
	void OnReloadComplete();
	
};
