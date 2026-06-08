// MyWeaponStruct.h

#pragma once

#include "Engine/DataTable.h"
#include "MyWeaponStruct.generated.h"

// BlueprintType : 이 struct를 블루프린트에서 “변수 타입으로 사용 가능”하게 함
USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeaponName;
	
	//소프트 참조를 이용해서 클래스를 가지고 있기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AActor> WeaponClass;
};

// struct vs class 차이점
	// 아무것도 안 쓰고, 코드를 짤 경우
		// struct 기본 - public
		// class 기본 - private
	// struct가 class보다 빌드에서 더 안정적임