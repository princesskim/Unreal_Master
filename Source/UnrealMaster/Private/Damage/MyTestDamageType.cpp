// MyTestDamageType.cpp

#include "Damage/MyTestDamageType.h"

UMyTestDamageType::UMyTestDamageType()
{
	bCausedByWorld = true;				
											// 트랩 데미지인데 false로 두면, 
											// 트랩을 설치한 플레이어가 공격자로 인식되어 몬스터가 쫓아올 수 있음
											// true → "그냥 환경에 의해 다쳤다" → 몬스터가 반응 안 함
	bScaleMomentumByMass = true;
	DamageImpulse = 5000.f;
}
