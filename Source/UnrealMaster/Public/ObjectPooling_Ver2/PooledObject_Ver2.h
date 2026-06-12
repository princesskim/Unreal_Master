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
	// 풀에 등록되거나 재사용될 때 필요한 정보를 외부에서 주입하는 함수
	// 인자로 매니저를 받아옴
	void Init();
	
	UFUNCTION(BlueprintCallable)
	void RecycleSelf();
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bIsPoolActive;
	
private:
	
	TObjectPtr<UObjectPoolSubsystem> PoolSubsystem;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

		
};
