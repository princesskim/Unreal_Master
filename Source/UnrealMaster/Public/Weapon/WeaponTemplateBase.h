// WeaponTemplateBase.h

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponTemplateBase.generated.h"


UCLASS()
class UNREALMASTER_API AWeaponTemplateBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:

	virtual void Fire() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Reload();
											// BlueprintNativeEvent
											// C++에서 기본 구현을 제공하면서, 블루프린트에서 재정의(Override)도 가능하게 만드는 UFUNCTION 지정자
											// 즉, 하이브리드형 함수
											// (cpp) — 반드시 _Implementation 접미사
protected:

	UFUNCTION(BlueprintNativeEvent)
	bool CheckAmmo();
	
	// 총을 어떻게 쏠지
	UFUNCTION(BlueprintImplementableEvent)
	void ProcessFiring();
											// BlueprintImplementableEvent
											// C++에서는 선언만 하고, 구현은 반드시 블루프린트에서만 하는 UFUNCTION 지정자
											// 블루프린트에서 구현 안 해도 에러 없음.
											// C++에서 구현 시도하면, 컴파일 에러남 !!!!!!
	// 이펙트 실행
	UFUNCTION(BlueprintImplementableEvent)
	void PlayEffects();
	
	// 총알 업데이트
	UFUNCTION(BlueprintNativeEvent)
	void UpdateAmmo();
};
