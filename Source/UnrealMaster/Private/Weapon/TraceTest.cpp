// TraceTest.cpp

#include "Weapon/TraceTest.h"
#include "Kismet/GameplayStatics.h"
#include "Damage/MyTestDamageType.h"

ATraceTest::ATraceTest()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void ATraceTest::BeginPlay()
{
	Super::BeginPlay();

}

void ATraceTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	StartAsyncTrace();
}


// 비동기 방식
// Kismet 말고, UWorld에서 가져오는 방식
// 자유로운 방식이라,트레이스 되는 물체의 콜리전을 바꿀 수도 있음
// 앞선 예시들과 다르게 시각적인 디버깅 불가능
void ATraceTest::StartAsyncTrace()
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false;
	
	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.WorldDynamic = ECR_Block;
															// 오브젝트 타입이 WorldDynamic인 애랑 부딪히면 
															// 강제로 Block 타입으로 바꿔버리기
	FTraceDelegate TraceDelegate;
	TraceDelegate.BindUObject(this, &ATraceTest::OnAsyncTraceComplete);
															// 델리게이트 설정 : 작업 완료 후 호출될 함수 연결 (바인딩 필요)
															// 바인딩 필요
	
	GetWorld()->AsyncLineTraceByChannel(
		EAsyncTraceType::Multi,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 1000.f,
		ECC_Visibility,
		QueryParams,
		ResponseParams,
		&TraceDelegate
	);
															// EAsyncTraceType::
																// Test는 뭔가에 맞았는지 안 맞았는지 여부만 반환 (Hit 정보 없음)
																// Single은 첫 번째로 맞은 오브젝트 1개의 Hit 정보 반환
															//  Test는 충돌 여부만 알면 될 때 가장 가볍게 쓸 수 있어서, 시야가 없는 몬스터의 존재 감지 용으로 굳
																//  안에 Data는 텅 빈상태
																//  누구인지를 확인하는 GetActor()를 찍을 경우에는 바로 크래쉬
}


void ATraceTest::OnAsyncTraceComplete(const FTraceHandle& Handle, FTraceDatum& Data)
{	
	for (const FHitResult& Hit : Data.OutHits)
	{
		AActor* HitActor = Hit.GetActor();
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Multi Hit Actor : %s"), *HitActor->GetName()));
		
		// 시각적인 디버그를 위해 그려주기
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 20.f, 12, FColor::Green, false, 2.f);
		
		UGameplayStatics::ApplyPointDamage(
			HitActor,								// 데미지를 받을 대상 액터
			50.f,									// 줄 데미지 수치
			GetActorForwardVector(), 
			Hit, 
			GetInstigatorController(), 
			this,									// 데미지를 유발한 컨트롤러 (누가 공격했는가)
			UMyTestDamageType::StaticClass()		// TSubClassOf -> 정적으로 넣어주기 / 물속성이면 물데미지타입을 넣는 등
			);
	}
}