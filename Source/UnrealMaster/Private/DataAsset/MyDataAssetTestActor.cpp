// MyDataAssetTestActor.cpp

#include "DataAsset/MyDataAssetTestActor.h"
#include "DataAsset/MyItemData.h"


AMyDataAssetTestActor::AMyDataAssetTestActor()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void AMyDataAssetTestActor::BeginPlay()
{
	Super::BeginPlay();
	StartLoading();
}

void AMyDataAssetTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyDataAssetTestActor::StartLoading()
{
	// ID 설정 안 되어 있으면
	if (!ItemToLoad.IsValid()) return;

	UAssetManager& AssetManager = UAssetManager::Get();

	// 번들을 비워두고 전체 로드하는 방법.
	TArray<FName> Bundles;
	// Bundles.Add(FName("Mesh")); // 이렇게 하면 Mesh 번들만 부름

	// CreateUObject를 사용해 완료 시 OnLoadFinished를 부르도록 예약.
	LoadingHandle = AssetManager.LoadPrimaryAsset(
		ItemToLoad,
		Bundles,
		FStreamableDelegate::CreateUObject(this, &AMyDataAssetTestActor::OnLoadFinished, ItemToLoad)
	);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("아이템 로드 시작..."));
}

void AMyDataAssetTestActor::OnLoadFinished(FPrimaryAssetId LoadedId)
{
	// ID을 보고 데이터에셋을 꺼내옴
	LoadedItem = Cast<UMyItemData>(UAssetManager::Get().GetPrimaryAssetObject(LoadedId));

	if (LoadedItem)
	{
		FString Msg = FString::Printf(TEXT("배달 완료! 아이템 이름: %s"), *LoadedItem->ItemMesh.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Msg);
		// 실제로 메시를 교체하거나, 수치 적용 등을 함
	}
}