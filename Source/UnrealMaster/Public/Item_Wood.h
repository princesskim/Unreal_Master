// Item_Wood.h

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "TestMyInterface.h"
#include "Item_Wood.generated.h"


UCLASS()
class UNREALMASTER_API AItem_Wood : public AItemBase, public ITestMyInterface
{
	GENERATED_BODY()
public:
	void OnFireDetected_Implementation(float Temperature, FVector HitLocation);
	
protected:
	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<class UParticleSystem> FireEffect;
};
