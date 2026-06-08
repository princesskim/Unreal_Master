// ItemManagerSubsystem.h

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/AssetManager.h"
#include "ItemManagerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemLoaded, FPrimaryAssetId, AssetId);
class UItemDataAsset;

UCLASS()
class UNREALMASTER_API UItemManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// 서브시스템 초기화 시 호출
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	// 아이템 비동기 로드 요청
	UFUNCTION(BlueprintCallable, Category = "ItemManager")
	void RequestAsyncLoadItem(FPrimaryAssetId AssetId);

	// 로드된 아이템 가져오기 (이미 로드된 경우만 반환)
	UFUNCTION(BlueprintCallable, Category = "ItemManager")
	class UItemDataAsset* GetItemData(FPrimaryAssetId AssetId);

	// 아이템 메모리 해제
	UFUNCTION(BlueprintCallable, Category = "ItemManager")
	void UnloadItem(FPrimaryAssetId AssetId);

	
	// 델리게이트
	UPROPERTY(BlueprintAssignable, Category="ItemManager")
	FOnItemLoaded OnItemLoaded;
	
protected:
	// 로딩 Handle을 보관할 변수 (이게 없으면 로딩 중 메모리에서 사라짐!)
	// AssetManager에게 요청을 할 때, 영수증처럼 핸들을 들고 있어야지 다시 나를 찾아옴
	// 둥둥 떠다니는 데이터는 GC가 지워버리기 때문에 TSharedPtr로 감싸기
	TMap<FPrimaryAssetId, TSharedPtr<FStreamableHandle>> LoadingHandles;
	
	// 로드 완료 후 실행될 함수
	// 로드 요청에 반환되는 것은 아이템이 아니고, ID임
	// 편의점 택배와 비슷 (ID 들고가서 내 물건 달라고 해야 함)
	void OnLoadCompleted(FPrimaryAssetId AssetId);
};
