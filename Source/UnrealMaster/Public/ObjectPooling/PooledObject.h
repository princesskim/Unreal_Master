// PooledObject.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PooledObject.generated.h"

class AMyObjectPool;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALMASTER_API UPooledObject : public UActorComponent
{
	GENERATED_BODY()

public:	
	// 풀에 등록되거나 재사용될 때 필요한 정보를 외부에서 주입하는 함수
	// 인자로 매니저를 받아옴
	void Init(AMyObjectPool* Owner);
	
	UFUNCTION(BlueprintCallable)
	void RecycleSelf();
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bIsPoolActive;
	
private:
	
	// 추종할 매니저
	TObjectPtr<AMyObjectPool> ObjectPool;
	
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
};
