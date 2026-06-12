// PoolCollectionDataAsset.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
class UPooledObjectDataAsset;
#include "PoolCollectionDataAsset.generated.h"


UCLASS()
class UNREALMASTER_API UPoolCollectionDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UPooledObjectDataAsset>> PoolDefinitions;
};
