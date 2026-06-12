/*
// ObjectPoolSubsystem.cpp

#include "ObjectPooling_Ver2/ObjectPoolSubsystem.h"

void UObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UObjectPoolSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UObjectPoolSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	// SpawnActor()를 호출할 때 사용할 추가 옵션들을 담는 구조체
	FActorSpawnParameters SpawnParams;
	
	// 설정된 액터 종류별로 풀 생성
	for (int32 PoolIndex = 0 ; PoolIndex < PooledObjectData.Num() ; ++PoolIndex)
	{
		// 실제 컴포넌트가 들어갈 공간
		FSingleObjectPool_Ver2 CurrentPoolIndex;
		
		// 정확한 이름 비교를 위해 '이름 규칙' 정해주기 (규칙 없으면 엔진에서 맘대로 바꾸기도 함)
		// 이름 정해주기 & 내가 생성한 이름을 최대한 사용해줘
		SpawnParams.Name = PooledObjectData[PoolIndex].ActorName;
		SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
		
		// 여러개가 스폰될 때, 겹치는 상황에서 어떡할건지 고르기
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		// 각 액터를 에디터에서 지정한 개수만큼 만들기
		for (int32 ObjectIndex = 0 ; ObjectIndex < PooledObjectData[PoolIndex].PoolSize ; ++ObjectIndex)
		{
			// 스폰 로직
			AActor* SpawnedActor = GetWorld()->SpawnActor(
				PooledObjectData[PoolIndex].ActorTemplate, 
				&FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
			
			// 월드 아웃라이너에 표시되는 이름 설정
			SpawnedActor->SetActorLabel(SpawnedActor->GetName());
			
			// 붙여줄 컴포넌트 만들고, 언리얼에 기능적 등록 -> 렌더, 틱, 물리 등
			UPooledObject_Ver2* PoolComp = NewObject<UPooledObject_Ver2>(SpawnedActor);
			PoolComp->RegisterComponent();
			
			// 컴포넌트 붙이고, 초기화
			SpawnedActor->AddInstanceComponent(PoolComp);
			PoolComp->Init(this);
			
			CurrentPoolIndex.PooledObjects.Add(PoolComp);
			
			SpawnedActor->SetActorHiddenInGame(true);
			SpawnedActor->SetActorEnableCollision(false);
			SpawnedActor->SetActorTickEnabled(false);
		}
		
		// ex. Bullet/100개 컴포넌트 뭉치를 저장
		Pools.Add(CurrentPoolIndex);
	}
}

void UObjectPoolSubsystem::Broadcast_PoolerCleanup()
{
	OnPoolerCleanup_Ver2.Broadcast();
}

AActor* UObjectPoolSubsystem::GetPooledActor(UPooledObjectDataAsset* PoolData)
{
	// 액터 종류 개수
	int32 PoolCount = PooledObjectData.Num();
	
	int32 CurrentPoolIndex = -1;
	
	for (int32 i = 0 ; i < PoolCount ; ++i)
	{
		// 이름 대조해서 인덱스 찾기
		if (PooledObjectData[i].ActorName == Name)
		{
			CurrentPoolIndex = i;
			break;
		}
	}
	
	if (CurrentPoolIndex == -1) return nullptr;

	// 존재한다면, 해당 인덱스 안에 컴포넌트가 몇 개 들어있는지
	int32 PooledObjectCount = Pools[CurrentPoolIndex].PooledObjects.Num();
	
	// 사용 대기 중(non-active)인 컴포넌트가 몇번째인지
	int32 FirstAvailable = -1;
	
	for (int32 i = 0 ; i < PooledObjectCount ; ++i)
	{
		if (Pools[CurrentPoolIndex].PooledObjects[i] != nullptr)
		{
			// 컴포넌트가 active하지 않으면
			if ( ! Pools[CurrentPoolIndex].PooledObjects[i]->bIsPoolActive)
			{
				FirstAvailable = i;
				break;
			}
		}
		else 
		{
			// [예외 상황] 해당 슬롯의 컨트롤러가 존재하지 않을 경우, 황급히 만들어주기
			ReGenerateItem(CurrentPoolIndex, i);
			FirstAvailable = i;
			break;
		}
	}
	
	// 사용할 수 있는 컴포넌트 인덱스를 얻음
	if (FirstAvailable >= 0)
	{
		UPooledObject_Ver2* ToReturnComp = Pools[CurrentPoolIndex].PooledObjects[FirstAvailable];
		ToReturnComp->bIsPoolActive = true;
		
		// 전체 회수용 델리게이트 바인딩
		OnPoolerCleanup_Ver2.AddUniqueDynamic(ToReturnComp, &UPooledObject_Ver2::RecycleSelf);
		
		// 컴포넌트가 붙어있는 액터를 데려와서, 활성화
		AActor* ToReturnActor = ToReturnComp->GetOwner();
		ToReturnActor->SetActorHiddenInGame(false);
		ToReturnActor->SetActorEnableCollision(true);
		ToReturnActor->SetActorTickEnabled(true);
		
		return ToReturnActor;
	}
	
	// FirstAvailable == -1인데 bCanGrow가 켜져있다면, 새로 스폰하고 바로 사용하기
	if (PooledObjectData[CurrentPoolIndex].bCanGrow)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Name = PooledObjectData[CurrentPoolIndex].ActorName;
		SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
		AActor* SpawnedActor = GetWorld()->SpawnActor(PooledObjectData[CurrentPoolIndex].ActorTemplate, 
			&FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
		SpawnedActor->SetActorLabel(SpawnedActor->GetName());
	
		UPooledObject_Ver2* PoolComp = NewObject<UPooledObject_Ver2>(SpawnedActor); // 컴포넌트 생성 & 아우터 지정
		PoolComp->RegisterComponent();
	
		SpawnedActor->AddInstanceComponent(PoolComp);
		PoolComp->Init(this);
	
		// Pools에 새로 만든 컴포넌트 추가
		Pools[CurrentPoolIndex].PooledObjects.Add(PoolComp);
	
		PoolComp->bIsPoolActive = true;
		
		// 전체 회수용 델리게이트 바인딩
		OnPoolerCleanup_Ver2.AddUniqueDynamic(PoolComp, &UPooledObject_Ver2::RecycleSelf);
	
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
	// 바인드 풀기
	OnPoolerCleanup_Ver2.RemoveDynamic(PoolCompRef, &UPooledObject_Ver2::RecycleSelf);
	
	// 비활성화 하기 
	PoolCompRef->bIsPoolActive = false;
	
	AActor* ReturningActor = PoolCompRef->GetOwner();
	ReturningActor->SetActorHiddenInGame(true);
	ReturningActor->SetActorEnableCollision(false);
	ReturningActor->SetActorTickEnabled(false);
}

void UObjectPoolSubsystem::ReGenerateItem(int32 PoolIndex, int32 PositionIndex)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = PooledObjectData[PoolIndex].ActorName;
	SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	AActor* SpawnedActor = GetWorld()->SpawnActor(PooledObjectData[PoolIndex].ActorTemplate, 
		&FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
	SpawnedActor->SetActorLabel(SpawnedActor->GetName());
	
	UPooledObject_Ver2* PoolComp = NewObject<UPooledObject_Ver2>(SpawnedActor);
	PoolComp->RegisterComponent();
	
	SpawnedActor->AddInstanceComponent(PoolComp);
	PoolComp->Init(this);
	
	// 컨트롤러가 없는 자리(PositionIndex)에 정확하게 삽입
	Pools[PoolIndex].PooledObjects[PositionIndex] = PoolComp;
	SpawnedActor->SetActorHiddenInGame(true);
	SpawnedActor->SetActorEnableCollision(false);
	SpawnedActor->SetActorTickEnabled(false);
}
*/

