// MyDataAssetTestActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/AssetManager.h"
#include "MyDataAssetTestActor.generated.h"

UCLASS()
class UNREALMASTER_API AMyDataAssetTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyDataAssetTestActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	// ID를 가지고 있다가 ID를 넘겨주면서 로드된 아이템을 가지고 오기
	// 로드되면 여기에 저장
	
	// 에디터에서 로드할 아이템을 선택할 칸 -- ID를 넣어주는 곳
	UPROPERTY(EditAnywhere, Category="ItemTest")
	FPrimaryAssetId ItemToLoad;

	// 로드된 결과물을 담아둘 변수
	UPROPERTY(VisibleAnywhere, Category="ItemTest")
	class UMyItemData* LoadedItem;
	
	// 테스트를 위해 숫자 1키를 누르면 실행되게 할 함수
	// 최초로 실행될 함수
	void StartLoading();
	
protected:
	// 로딩 Handle을 보관할 변수 (이게 없으면 로딩 중 메모리에서 사라짐!)
	// AssetManager에게 요청을 할 때, 영수증처럼 핸들을 들고 있어야지 다시 나를 찾아옴
	// 둥둥 떠다니는 데이터는 GC가 지워버리기 때문에 TSharedPtr로 감싸기
	TSharedPtr<FStreamableHandle> LoadingHandle;
	
	// 로드 완료 후 실행될 함수
	// 로드 요청에 반환되는 것은 아이템이 아니고, ID임
	// 편의점 택배와 비슷
	void OnLoadFinished(FPrimaryAssetId LoadedID);
};
