// GunStatComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALMASTER_API UGunStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGunStatComponent();

	// 최대 탄약 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxAmmo;

	// 현재 남은 탄약 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CurrentAmmo;

	// 한 번 발사 시 나가는 총알 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BulletsPerFire;

	// 총알이 퍼지는 각도 (권총/저격 = 0, 샷건 = 45)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpreadAngle;

	// 총알 사거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Range;

	// 총알 한 발당 데미지
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;

	// 발사 간격 (초)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FireRate;

	// 재장전 시간 (초)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ReloadTime;
		
	// 발사 시 카메라가 위로 튀는 각도
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RecoilPitch;

	// 발사 시 카메라가 좌우로 튀는 각도
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RecoilYaw;
};
