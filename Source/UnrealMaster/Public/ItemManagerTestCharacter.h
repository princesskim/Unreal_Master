// ItemManagerTestCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/PrimaryAssetId.h"				// PrimaryAssetId 사용을 위해 필요
#include "ItemManagerTestCharacter.generated.h"

class UItemDataAsset;

UCLASS()
class UNREALMASTER_API AItemManagerTestCharacter : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	// 실습을 위해 에디터에서 선택할 아이템 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Test")
	FPrimaryAssetId ItemToEquip;
	
	// 아이템 장착 프로세스 시작
	UFUNCTION(BlueprintCallable, Category="Item Test")
	void EquipItem(FPrimaryAssetId ItemId);

	// 로드 완료 시 호출될 함수
	UFUNCTION()
	void OnEquipReady(FPrimaryAssetId LoadedId);
	
private:
	//현재 장착된 아이템 데이터를 보관
	UPROPERTY()
	class UItemDataAsset* CurrentEquippedItem;
};
