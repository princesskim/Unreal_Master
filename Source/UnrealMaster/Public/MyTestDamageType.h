// MyTestDamageType.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "MyTestDamageType.generated.h"

UCLASS()
class UNREALMASTER_API UMyTestDamageType : public UDamageType
{
	GENERATED_BODY()
	
public:
	UMyTestDamageType();
};

/* 
UDamageType 주요 멤버 변수들
* 주로 이렇게 쓸 뿐이지, 팀규칙에 맞게 바꿔서 활용 가능

	bCausedByWorld          // 월드 자체가 원인인지 여부
							// true : 낙사, 트랩, 용암 등 환경에 의한 사망
							// false : 플레이어, 몬스터 등 액터에 의한 사망

	bScaleMomentumByMass    // 피격 시 밀려나는 힘을 무게에 따라 보정할지 여부
							// true : 무거운 오브젝트는 덜 밀리고, 가벼운 오브젝트는 더 밀림
							// false : 무게 무관하게 동일한 힘으로 밀림

	DamageImpulse           // 데미지 발생 시 피격 물리 객체를 밀어내는 힘의 크기
							// 총알에 맞아 뒤로 밀리는 넉백 효과 등에 활용

	DestructibleImpulse     // 파괴 가능한 메시(Destructible Mesh)에 가해지는 충격량
							// 폭발로 벽이 부서질 때 조각이 날아가는 정도

	DestructibleDamageSpreadScale   // 파괴 가능한 메시에서 데미지가 주변으로 퍼지는 범위 배율
									// 값이 클수록 충격이 더 넓게 퍼져 더 많은 부위가 부서짐
*/