// TestMyInterface.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TestMyInterface.generated.h"

// U 타입 : 리플렉션 시스템에 인터페이스가 존재함을 알려주기 위한 껍데기
UINTERFACE(MinimalAPI)
class UTestMyInterface : public UInterface
{
	GENERATED_BODY()
};

// I 순수 타입 : 실제로 함수를 선언하고 상속받아 구현하는 클래스
// 순수 타입(일반 C++ 클래스)이기 때문에 다중상속 가능
class UNREALMASTER_API ITestMyInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interface")
	void OnFireDetected(float Temperature, FVector HitLocation);
};
