// WeaponSandboxBase.h

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponSandboxBase.generated.h"


UCLASS()
class UNREALMASTER_API AWeaponSandboxBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:

	virtual void Fire() override;				// 블루프린트에서 Fire() 못 쓰니까
	
	UFUNCTION(BlueprintImplementableEvent)
	void SandboxFire();							// 위임하는 함수 만들기
												// 구현부 만들면 안 됨
	
	UFUNCTION(BlueprintCallable)
	void Reload();
	
protected:
	
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
};
