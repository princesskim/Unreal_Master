// WeaponBox.cpp

#include "SoftRef/WeaponBox.h"
#include "SoftRef/MyWeaponStruct.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

AWeaponBox::AWeaponBox()
{
 	PrimaryActorTick.bCanEverTick = true;

}


void AWeaponBox::BeginPlay()
{
	Super::BeginPlay();
	OpenBox();
}


void AWeaponBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBox::OpenBox()
{
	if (!WeaponTable) return;
	
	TArray<FWeaponData*> AllWeapons;
	
	//Row 데이터 전부 가져오기
	WeaponTable->GetAllRows<FWeaponData>(TEXT(""), AllWeapons);
	
	// null 체크
	if (AllWeapons.Num() == 0) return;
	
	// 데이터 테이블에서 랜덤하게 행 하나를 가져옴
	FWeaponData* SelectedWeapon = AllWeapons[FMath::RandRange(0, AllWeapons.Num()-1)];
	
	if (SelectedWeapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Async Load Requesting!"));
		
		// 직접 AsyncLoad를 호출하는 대신, 에셋 매니저에게 요청
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		
		// RequestAsyncLoad는 경로(Path)를 인자로 받음
		// 바인딩!
		Streamable.RequestAsyncLoad(SelectedWeapon->WeaponClass.ToSoftObjectPath(),
				FStreamableDelegate::CreateUObject(this, &AWeaponBox::OnWeaponAsyncSpawned, SelectedWeapon->WeaponClass));
	}
}

// 로드가 완료된 시점에 엔진이 자동으로 이 함수를 호출
void AWeaponBox::OnWeaponAsyncSpawned(TSoftClassPtr<AActor> WeaponClassPtr)
{
	// .Get() : 이미 로드되어 메모리에 올라온 UObject를 꺼내기
	UClass* LoadedClass = WeaponClassPtr.Get();
		
	if (LoadedClass)
	{
		// 월드에 소환
		FActorSpawnParameters SpawnParams;
		// 큰 에셋이어도 무조건 스폰할 수 있는 기능
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		GetWorld()->SpawnActor<AActor>(LoadedClass, GetActorLocation() + FVector(0.f, 0.f, 100.f), FRotator::ZeroRotator, SpawnParams);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s Async Loaded!"), *LoadedClass->GetName()));
	}
}
