// ItemBase.cpp

#include "ItemBase.h"
#include "MyActorComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AItemBase::AItemBase()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		HealthComponent = PlayerCharacter->FindComponentByClass<UMyActorComponent>();
		
		if (HealthComponent)
		{
			// Dynamic, Multicast이기 때문에 AddDynamic
			HealthComponent->OnHealthDead.AddDynamic(this, &AItemBase::OnPlayerDead);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No MyActorComponent in PlayerCharacter!"));
		}
	}
}

void AItemBase::OnPlayerDead(AController* InstigatorController)
{
	Destroy();
	UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter Died."));
	/*
	 * 파티클 안 사라지는 거 해결하는 법
	 * UGameplayStatics::SpawnEmitterAtLocation의 반환값을 
	 * 따로 UParticleSystemComponent* 인스턴스 변수로 저장해뒀다가 
	 * EndPlay에서 따로 Destroy
	 */
}

void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

