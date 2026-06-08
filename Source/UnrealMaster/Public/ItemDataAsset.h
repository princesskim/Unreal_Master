// ItemDataAsset.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"


UCLASS()
class UNREALMASTER_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FPrimaryAssetType ItemType;
	
	UPROPERTY(EditAnywhere, Category="Stats")
	float AttackDamage;
	
	// 무거운 에셋은 SoftObjectPtr로 선언해서 로드 시점을 제어
	// meta : 번들을 설정해서 2개의 SkeletalMesh 중 하나만 부를 수 있도록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visual")
	TSoftObjectPtr<USkeletalMesh> ItemMesh;
	
	// ID를 반환하는 함수 오버라이드
	// 에셋 매니저가 이 파일을 식별할 수 있는 ID 규칙을 정함
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(ItemType, GetFName());
	}
};
