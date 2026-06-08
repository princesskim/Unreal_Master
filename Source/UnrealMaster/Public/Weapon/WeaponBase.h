// WeaponBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USceneComponent;
class UArrowComponent;

UCLASS()
class UNREALMASTER_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> FirePoint;		// 총을 쏠 때, 총알이 어디서 나가야 하는지 보여줌 (방향 가리키는 기능)
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void Fire();
	
protected:

	// 한 발 당 소모되는 탄약 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmmoPerFire;
	
	// 남은 탄약 수
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	int32 CurrentAmmo;
	
	// 탄약 보유량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmo;
	
	// 발사 속도 (단위 시간당 몇 발을 쏠 수 있는지)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RoF;											// 1.f/RoF : 발사 간격
	
	// 유효 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;
	
	// 데미지 양
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamagePerHit;

	// 쏠 수 있는지 상태 체크
	UPROPERTY(BlueprintReadWrite)
	bool CanFire;
	
	// 연사 속도 제어를 위한 핸들
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle TimerFireDelay;
	
	// 바인딩 해야하기 때문에 매크로 필수
	UFUNCTION()
	void HandleFireDelay();
};


/*
[1] 함수 포인터 방식
-> 엄밀히 말하면, 함수 포인터 방식으로 바인딩할 때는 UFUNCTION()이 없어도 동작

GetWorldTimerManager().SetTimer(
	TimerFireDelay,
	this,
	&AWeaponBase::HandleFireDelay,  // 함수 포인터
	RoF,
	false
);


[2] 델리게이트(Delegate) 방식
-> 델리게이트(Delegate) 방식으로 바인딩할 때는 매크로 필수
-> BindUFunction()은 내부적으로 리플렉션 시스템을 통해 함수 이름으로 함수를 검색하기 때문

FTimerDelegate Delegate;
Delegate.BindUFunction(this, FName("HandleFireDelay")); // 함수 이름(문자열)으로 찾음
GetWorldTimerManager().SetTimer(TimerFireDelay, Delegate, RoF, false);
 */
