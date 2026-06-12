// PooledObjectData.h

#pragma once

#include "CoreMinimal.h"
#include "PooledObjectData.generated.h"

USTRUCT(BlueprintType)
struct FPooledObjectData
{
	GENERATED_BODY()
	
	FPooledObjectData()
	{
		ActorTemplate = nullptr;
		PoolSize = 1;
		bCanGrow = false;
		ActorName = "default";
	}
	
	//어떤 액터를 스폰할 것인가.
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorTemplate;
	
	//몇개를 스폰할 것인가
	UPROPERTY(EditAnywhere)
	int32 PoolSize;
	
	// PoolSize가 50개인데, 100개 요청이 들어오면, 부족한 개수를 그 즉시 만들건지
	UPROPERTY(EditAnywhere)
	bool bCanGrow;
	
	// 가지고 있는 여러 액터들의 이름을 대조해야 해서 필요함
	UPROPERTY(EditAnywhere)
	FName ActorName;
};