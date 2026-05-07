// FireDamageType.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "FireDamageType.generated.h"

UCLASS()
class UNREALMASTER_API UFireDamageType : public UDamageType
{
	GENERATED_BODY()
	
public:
	UFireDamageType();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BurnDuration; // 화상 지속 시간
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ArmorPenetration; // 방어력 관통 (20%)
};
