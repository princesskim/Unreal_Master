// Shotgun.h

#pragma once

#include "CoreMinimal.h"
#include "BaseGun.h"
#include "Shotgun.generated.h"

UCLASS()
class UNREALMASTER_API AShotgun : public ABaseGun
{
	GENERATED_BODY()
	
public:
	AShotgun();
	
	virtual bool SandboxFire_Implementation() override;
										// BP에서 오버라이드하면 SandboxFire() 이벤트 노드로 구현됨
										// C++에서는 SandboxFire_Implementation() 으로 오버라이드
	
										// 부모에서 void SandboxFire(); 이지만
										// BlueprintNativeEvent 는 내부적으로 virtual 로 만들어줌
										// 즉, 언리얼 리플렉션 시스템이 만들어준 구현부를 오버라이드
};
