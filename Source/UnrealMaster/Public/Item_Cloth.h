// Item_Cloth.h

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"

// 인터페이스 헤더를 .generated.h 보다 위에 인클루트 해주기
#include "TestMyInterface.h"
#include "Item_Cloth.generated.h"

UCLASS()
class UNREALMASTER_API AItem_Cloth : public AItemBase, public ITestMyInterface
{
	GENERATED_BODY()
public:
	void OnFireDetected_Implementation(float Temperature, FVector HitLocation);
	
protected:
	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<class UParticleSystem> FireEffect;
};
