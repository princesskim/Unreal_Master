// PooledObjectDataAsset.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PooledObjectDataAsset.generated.h"


UCLASS()
class UNREALMASTER_API UPooledObjectDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	// 풀 매니저가 이 파일을 식별할 수 있는 ID 규칙 정함
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(PoolType, GetFName());
	}
	
	// 현재는 카테고리가 PooledObject로 고정이지만 나중에 Projectile, Effect, NPC.. 같은 상위 카테고리로 확장 가능
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ObjectPool")
	FPrimaryAssetType PoolType = TEXT("PooledObject");
	
	// 풀에 생성해둘 액터 클래스
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorTemplate = nullptr;
	
	//몇개를 스폰할 것인가
	UPROPERTY(EditAnywhere)
	int32 PoolSize = 1;
	
	// PoolSize가 50개인데, 100개 요청이 들어오면, 부족한 개수를 그 즉시 만들건지
	UPROPERTY(EditAnywhere)
	bool bCanGrow = false;
	
};