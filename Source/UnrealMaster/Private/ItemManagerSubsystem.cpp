// ItemManagerSubsystem.cpp

#include "ItemManagerSubsystem.h"
#include "ItemDataAsset.h"

void UItemManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("Item Manager Subsystem Initialized!"));
}

void UItemManagerSubsystem::RequestAsyncLoadItem(FPrimaryAssetId AssetId)
{
	if (!AssetId.IsValid()) return;
	
	// 이미 로드 중이거나 로드 완료되었는지 확인
	if (LoadingHandles.Contains(AssetId) && LoadingHandles[AssetId]->IsActive())
	{
		return;
	}
	
	UAssetManager& Manager = UAssetManager::Get();
	
	// 번들을 비워두고 전체 로드하는 방법.
	TArray<FName> Bundles;
	
	// 비동기 로드 시작
	TSharedPtr<FStreamableHandle> Handle = Manager.LoadPrimaryAsset(
		AssetId, 
		Bundles,
		FStreamableDelegate::CreateUObject(this, &UItemManagerSubsystem::OnLoadCompleted, AssetId)
		);
	
	LoadingHandles.Add(AssetId, Handle);
}

class UItemDataAsset* UItemManagerSubsystem::GetItemData(FPrimaryAssetId AssetId)
{
	// 에셋 매니저를 통해 메모리에 올라온 DA 객체 포인터를 즉시 반환
	return UAssetManager::Get().GetPrimaryAssetObject<UItemDataAsset>(AssetId);
}

void UItemManagerSubsystem::UnloadItem(FPrimaryAssetId AssetId)
{
	if (LoadingHandles.Contains(AssetId))
	{
		LoadingHandles.Remove(AssetId);
		UAssetManager::Get().UnloadPrimaryAsset(AssetId);
		UE_LOG(LogTemp, Log, TEXT("Item Unloaded: %s"), *AssetId.ToString());
	}
}

void UItemManagerSubsystem::OnLoadCompleted(FPrimaryAssetId AssetId)
{
	UE_LOG(LogTemp, Log, TEXT("Item Loaded via Subsystem: %s"), *AssetId.ToString());
	
	// 로드 완료 알림
	if (OnItemLoaded.IsBound())
	{
		OnItemLoaded.Broadcast(AssetId);
	}
}
