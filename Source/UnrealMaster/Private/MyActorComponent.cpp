// MyActorComponent.cpp

#include "MyActorComponent.h"

UMyActorComponent::UMyActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	MaxHealth = 100.f;
}


void UMyActorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
	
	OnHealthDamaged.Broadcast(
		CurrentHealth,
		MaxHealth,
		0.f
	);
	
	/*UE_LOG(
		LogTemp,
		Warning,
		TEXT("HealthComponent BeginPlay Owner=%s Health=%f"),
		*GetOwner()->GetName(),
		CurrentHealth
	);*/
	// 신호 받기
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UMyActorComponent::DamageTake);
}


void UMyActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);			// 반드시 부모꺼 호출하기!!
	
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("HP : %f"), CurrentHealth));
}

void UMyActorComponent::DamageTake(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* Instigator, AActor* Causer)
{
	float FinalDamage = FMath::Min(Damage, CurrentHealth);	// 오버킬 차단
	CurrentHealth -= FinalDamage;
	
	OnHealthDamaged.Broadcast(CurrentHealth, MaxHealth, FinalDamage);
	if (CurrentHealth <= 0.f)
	{
		// 신호 보내기
		OnHealthDead.Broadcast(Instigator);	// 때린 애의 정보를 넘김 -> 킬로그, 경험치 주기 등 가능
	}
}

