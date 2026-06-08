// TestActor.cpp

#include "SoftRef/TestActor.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

ATestActor::ATestActor()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	LoadWithSoftPtr();
}

void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestActor::LoadWithSoftPtr()
{
	if (MySoftMesh.IsNull()) return;
	
	UStaticMesh* FinalMesh = nullptr;
	
	if (MySoftMesh.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Already Loaded."));
		OnLoadCompleted();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("AsyncLoad Started! (Using AssetManager)"));
		
		// 직접 AsyncLoad를 호출하는 대신, 에셋 매니저에게 요청
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		
		// RequestAsyncLoad는 경로(Path)를 인자로 받음
		Streamable.RequestAsyncLoad(MySoftMesh.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &ATestActor::OnLoadCompleted));
	}
}

void ATestActor::OnLoadCompleted()
{
	// 비동기가 끝나면, 안의 내용물을 가져와서 채워줌
	UStaticMesh* FinalMesh = MySoftMesh.Get();
	
	if (FinalMesh)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AsyncLoad Success : %s"), *FinalMesh->GetName()));
	}
}

