// ObjectPoolSubsystem.h

#pragma once

#include "CoreMinimal.h"
#include "PoolCollectionDataAsset.h"
#include "Subsystems/WorldSubsystem.h"
#include "PooledObject_Ver2.h"		// 오브젝트에 붙을 컴포넌트 헤더 인클루드
class UPooledObjectDataAsset;		// 데이터에셋
#include "ObjectPoolSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FSingleObjectPool_Ver2
{
	GENERATED_BODY()
	
	// 컴포넌트들을 담은 배열
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UPooledObject_Ver2>> PooledObjects;
};

UCLASS()
class UNREALMASTER_API UObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	void SetActorActive(AActor* Actor, bool bActive);
	
	// AMyObjectPool::BeginPlay() 로직을 여기로
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
public:	
	
	// 집 나갔던 액터들 전부 회수
	UFUNCTION(BlueprintCallable)
	void PoolerCleanup();
	
	// 외부에서 원하는 액터 이름을 대고, PooledObjectData 중 원하는 풀오브젝트 1개를 꺼내가는 함수
	UFUNCTION(BlueprintCallable)
	AActor* GetPooledActor(UPooledObjectDataAsset* PoolData);
	
	// 액터가 Pool 컴포넌트를 가지고 있다면 풀에 반환할 수 있도록 작업
	UFUNCTION(BlueprintCallable)
	void RecycleActor(AActor* PooledActor);
	
	// 최종 회수
	UFUNCTION(BlueprintCallable)
	void RecyclePooledObject(UPooledObject_Ver2* PoolCompRef);
	
	// 레시피들의 모음
	TObjectPtr<UPoolCollectionDataAsset> PoolCollection;
	
	
	
protected:
	
	// 실제 생성된 풀링 오브젝트들을 종류별로 관리하는 컨테이너
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<TObjectPtr<UPooledObjectDataAsset>, FSingleObjectPool_Ver2> Pools;
	
private:
	// 풀 확장이 허용된 경우 새로운 오브젝트를 생성하여 추가
	void ReGenerateItem(UPooledObjectDataAsset* PoolData, FSingleObjectPool_Ver2* Pool, int32 PositionIndex);
};
