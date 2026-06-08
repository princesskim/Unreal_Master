// MyItemData.h

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserDataSource.h"
#include "Engine/DataAsset.h"
#include "MyItemData.generated.h"

UCLASS()
class UNREALMASTER_API UMyItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	// 에셋 매니저가 이 파일을 식별할 수 있는 ID 규칙을 정함
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(ItemType, GetFName());
    }
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FPrimaryAssetType ItemType;
	
	// Struct 처럼 활용하면 됨
	// 무거운 에셋도 넣을 수 있음
	
	// 무거운 데이터는 반드시 TSoftObjectPtr로 선언하기 (번들 설정 포함)
	// meta : 번들을 설정해서 2개의 SkeletalMesh 중 하나만 부를 수 있도록
	UPROPERTY(EditAnywhere, Category="Visual", meta = (AssetBundles = "Mesh"))
	TSoftObjectPtr<USkeletalMesh> ItemMesh;
	
	UPROPERTY(EditAnywhere, Category="Visual", meta = (AssetBundles = "Character"))
	TSoftObjectPtr<USkeletalMesh> ItemMesh2;
	
	UPROPERTY(EditAnywhere, Category="Stats")
	float AttackPower;
	
};
