// EnemyCharacter.cpp

#include "Character/EnemyCharacter.h"

#include "Kismet/GameplayStatics.h"

AEnemyCharacter::AEnemyCharacter()
{
 	PrimaryActorTick.bCanEverTick = false;
	SightTraceInterval = 0.1f;
	SightAngle = 45.f;
	SightRange = 1000.f;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	TraceDelegate.BindUObject(this, &AEnemyCharacter::OnAsyncTraceComplete);
	
	GetWorldTimerManager().SetTimer(
		SightTraceTimerHandle, this,
		&AEnemyCharacter::StartAsyncTrace,
		SightTraceInterval, true
	);
}

void AEnemyCharacter::StartAsyncTrace()
{
	DrawDebugLine(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * SightRange,
		FColor::Blue, false, 0.1f
	);
	
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!Player) return;
	
	FVector PlayerLocation = Player->GetActorLocation();
	FVector ToPlayer = (PlayerLocation - GetActorLocation()).GetSafeNormal();
	float DotProduct = FVector::DotProduct(GetActorForwardVector(), ToPlayer);
	
	if (DotProduct <= FMath::Cos(FMath::DegreesToRadians(SightAngle))) return;
	if (FVector::Distance(PlayerLocation, GetActorLocation()) > SightRange) return;
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false;
	
	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.WorldDynamic = ECR_Block;
	
	// FTraceDelegate TraceDelegate; // 매벙 생성 안 하고, 헤더에 선언하는 것으로 변경 
	// TraceDelegate.BindUObject(this, &AEnemyCharacter::OnAsyncTraceComplete); //BeginPlay()에서 1회 바인딩
	
	GetWorld()->AsyncLineTraceByChannel(
		EAsyncTraceType::Multi,
		GetActorLocation(),
		GetActorLocation() + ToPlayer * SightRange,
		ECC_Pawn,
		QueryParams,
		ResponseParams,
		&TraceDelegate
		);
}

void AEnemyCharacter::OnAsyncTraceComplete(const FTraceHandle& Handle, FTraceDatum& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("TraceComplete! HitCount: %d"), Data.OutHits.Num());
    
	for (const FHitResult& Hit : Data.OutHits)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) continue;
		
		ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (HitActor != Player) continue;
		
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Enemy detected %s"), *HitActor->GetName()));
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 20.f, 12, FColor::Green, false, 2.f);
		
	}
}

