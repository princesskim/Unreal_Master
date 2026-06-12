// PooledObject_Ver2.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
class UObjectPoolSubsystem;
#include "PooledObject_Ver2.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALMASTER_API UPooledObject_Ver2 : public UActorComponent
{
	GENERATED_BODY()

public:	
	void Init();
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bIsPoolActive;
	
private:
	
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
};
