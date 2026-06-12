// MyObjectPool.h
// 풀 매니저 역할 : 월드 서브시스템으로 만드는 것이 더 좋음. 일단은 액터로 진행

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledObject.h"		// 오브젝트에 붙을 컴포넌트 헤더 인클루드
#include "MyObjectPool.generated.h"

struct FPooledObjectData;


// 풀 매니저가 풀에 들어있는 오브젝트들를 전부 회수하고 싶을 때 사용할 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPoolerCleanerSignature);


USTRUCT(BlueprintType)
struct FSingleObjectPool
{
	GENERATED_BODY()
	
	// 컴포넌트들을 담은 배열
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UPooledObject>> PooledObjects;
};


UCLASS()
class UNREALMASTER_API AMyObjectPool : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:	
	// 델리게이트 객체
	FPoolerCleanerSignature OnPoolerCleanup;
	
	// 집 나갔던 액터들 전부 회수
	UFUNCTION(BlueprintCallable)
	void Broadcast_PoolerCleanup();
	
	// 외부에서 원하는 액터 이름을 대고, PooledObjectData 중 원하는 풀오브젝트 1개를 꺼내가는 함수
	UFUNCTION(BlueprintCallable)
	AActor* GetPooledActor(FName Name);
	
	// 액터가 Pool 컴포넌트를 가지고 있다면 풀에 반환할 수 있도록 작업
	UFUNCTION(BlueprintCallable)
	void RecycleActor(AActor* PooledActor);
	
	// 최종 회수
	UFUNCTION(BlueprintCallable)
	void RecyclePooledObject(UPooledObject* PoolCompRef);
	
	// 에디터에서 생성할 때, 지정해줄 구조체 모음
	// 어떤 액터를 몇 개씩 풀링할지 정의하는 설정 정보 - Bullet/100개, Rocket/20개 ...
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPooledObjectData> PooledObjectData;
	
protected:
	
	// 실제 생성된 풀링 오브젝트들을 종류별로 관리하는 컨테이너
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FSingleObjectPool> Pools;
	
private:
	// 풀 확장이 허용된 경우 새로운 오브젝트를 생성하여 추가
	void ReGenerateItem(int32 PoolIndex, int32 PositionIndex);
};
