// ObjectPoolSubsystem.cpp

#include "ObjectPooling_Ver2/ObjectPoolSubsystem.h"

#include "ObjectPooling_Ver2/PooledObjectDataAsset.h"

void UObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// 레시피들의 모음 - 경로로 가져오기 (하드코딩)
	PoolCollection = LoadObject<UPoolCollectionDataAsset>(nullptr, TEXT("/Game/ObjectPoolingTest/DA_PoolCollection.DA_PoolCollection"));
}

void UObjectPoolSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UObjectPoolSubsystem::SetActorActive(AActor* Actor, bool bActive)
{
	if (!Actor) return;

	Actor->SetActorHiddenInGame(!bActive);
	Actor->SetActorEnableCollision(bActive);
	Actor->SetActorTickEnabled(bActive);
}

void UObjectPoolSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	// 월드마다 다시 호출되기 때문에 컨테이너 초기화 필요
	Pools.Empty();
	
	
	// SpawnActor()를 호출할 때 사용할 추가 옵션들을 담는 구조체
	FActorSpawnParameters SpawnParams;
	
	if (! PoolCollection) return;
	
	// 설정된 액터 종류별로 풀 생성
	for (UPooledObjectDataAsset* PoolData: PoolCollection->PoolDefinitions)
	{
		if (! PoolData || ! PoolData->ActorTemplate) continue;
		
		// 실제 컴포넌트가 들어갈 공간
		FSingleObjectPool_Ver2 Pool;
		
		// 데이터 에셋을 사용하면서 액터 이름 비교 과정이 빠짐
		
		// 여러개가 스폰될 때, 겹치는 상황에서 어떡할건지 고르기
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		// 각 액터를 에디터에서 지정한 개수만큼 만들기
		for (int32 ObjectIndex = 0 ; ObjectIndex < PoolData->PoolSize ; ++ObjectIndex)
		{
			// 스폰 로직
			AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
				PoolData->ActorTemplate, 
				FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			
			// 월드 아웃라이너에 표시되는 이름 설정
			SpawnedActor->SetActorLabel(SpawnedActor->GetName());
			
			// 컴포넌트 붙이고, 초기화
			// 붙여줄 컴포넌트 만들고, 언리얼에 기능적 등록 -> 렌더, 틱, 물리 등
			UPooledObject_Ver2* PoolComp = NewObject<UPooledObject_Ver2>(SpawnedActor);
			SpawnedActor->AddInstanceComponent(PoolComp);
			PoolComp->RegisterComponent();
			
			PoolComp->Init();
			
			Pool.PooledObjects.Add(PoolComp);
			
			SetActorActive(SpawnedActor, false);
		}
		
		// ex. Bullet/100개 컴포넌트 뭉치를 저장
		Pools.Add(PoolData, Pool);
	}
}

void UObjectPoolSubsystem::PoolerCleanup()
{
	for (auto& Pair : Pools)
	{
		for (UPooledObject_Ver2* Comp : Pair.Value.PooledObjects)
		{
			if (Comp)
			{
				Comp->bIsPoolActive = false;
				
				AActor* Owner = Comp->GetOwner();
				if (Owner)
				{
					SetActorActive(Owner, false);
				}
			}
		}
	}
}

AActor* UObjectPoolSubsystem::GetPooledActor(UPooledObjectDataAsset* PoolData)
{
	FSingleObjectPool_Ver2* Pool = Pools.Find(PoolData);
	if (Pool == nullptr) return nullptr;

	// 존재한다면, 해당 인덱스 안에 컴포넌트가 몇 개 들어있는지
	int32 PooledObjectCount = Pool->PooledObjects.Num();
	
	// 사용 대기 중(non-active)인 컴포넌트가 몇번째인지
	int32 FirstAvailable = -1;
	
	for (int32 i = 0 ; i < PooledObjectCount ; ++i)
	{
		if (Pool->PooledObjects[i] != nullptr)
		{
			// 컴포넌트가 active하지 않으면
			if ( ! Pool->PooledObjects[i]->bIsPoolActive)
			{
				FirstAvailable = i;
				break;
			}
		}
		else 
		{
			// [예외 상황] 해당 슬롯의 컨트롤러가 존재하지 않을 경우, 황급히 만들어주기
			ReGenerateItem(PoolData, Pool, i);
			FirstAvailable = i;
			break;
		}
	}
	
	// 사용할 수 있는 컴포넌트 인덱스를 얻음
	if (FirstAvailable >= 0)
	{
		UPooledObject_Ver2* ToReturnComp = Pool->PooledObjects[FirstAvailable];
		ToReturnComp->bIsPoolActive = true;

		// 컴포넌트가 붙어있는 액터를 데려와서, 활성화
		AActor* ToReturnActor = ToReturnComp->GetOwner();
		SetActorActive(ToReturnActor, true);
		
		return ToReturnActor;
	}
	
	// FirstAvailable == -1인데 bCanGrow가 켜져있다면, 새로 스폰하고 바로 사용하기
	if (PoolData->bCanGrow)
	{
		if (! PoolData || ! PoolData->ActorTemplate) return nullptr;
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(PoolData->ActorTemplate, 
			FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		SpawnedActor->SetActorLabel(SpawnedActor->GetName());
	
		UPooledObject_Ver2* PoolComp = NewObject<UPooledObject_Ver2>(SpawnedActor); // 컴포넌트 생성 & 아우터 지정
		SpawnedActor->AddInstanceComponent(PoolComp);
		PoolComp->RegisterComponent();
	
		PoolComp->Init();
	
		// Pools에 새로 만든 컴포넌트 추가
		Pool->PooledObjects.Add(PoolComp);
	
		PoolComp->bIsPoolActive = true;
		SetActorActive(SpawnedActor, true);
		
		return SpawnedActor;
	}
	else
	{
		return nullptr;
	}
}

void UObjectPoolSubsystem::RecycleActor(AActor* PooledActor)
{
	if (UPooledObject_Ver2* PoolCompRef = Cast<UPooledObject_Ver2>(PooledActor->GetComponentByClass(UPooledObject_Ver2::StaticClass())))
	{
		RecyclePooledObject(PoolCompRef);
	}
}

void UObjectPoolSubsystem::RecyclePooledObject(UPooledObject_Ver2* PoolCompRef)
{
	// 비활성화 하기 
	PoolCompRef->bIsPoolActive = false;
	
	AActor* ReturningActor = PoolCompRef->GetOwner();
	SetActorActive(ReturningActor, false);
}

void UObjectPoolSubsystem::ReGenerateItem(UPooledObjectDataAsset* PoolData, FSingleObjectPool_Ver2* Pool, int32 PositionIndex)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(PoolData->ActorTemplate, 
		FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	SpawnedActor->SetActorLabel(SpawnedActor->GetName());
	
	UPooledObject_Ver2* PoolComp = NewObject<UPooledObject_Ver2>(SpawnedActor);
	SpawnedActor->AddInstanceComponent(PoolComp);
	PoolComp->RegisterComponent();
	
	PoolComp->Init();
	
	// 컨트롤러가 없는 자리(PositionIndex)에 정확하게 삽입
	if (Pool->PooledObjects.IsValidIndex(PositionIndex))
	{
		Pool->PooledObjects[PositionIndex] = PoolComp;
	}
	else
	{
		Pool->PooledObjects.Add(PoolComp);
	}
	
	SetActorActive(SpawnedActor, false);
}

