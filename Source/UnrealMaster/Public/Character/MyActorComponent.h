// MyActorComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyActorComponent.generated.h"

// 1대 다수로 블루프린트까지 지원하는 "죽었을 때", "데미지를 입었을 때"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthDeadSignature, AController*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHealthDamagedSignature, float, NewHealth, float, MaxHealth, float, HealthChange);
// 1번째 인자 : 델리게이트 이름은 반드시 F부터 시작
// 2, 3번째 인자 : 매개변수 유형, 이름



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALMASTER_API UMyActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMyActorComponent();
	
	UPROPERTY(BlueprintAssignable)
	FHealthDeadSignature OnHealthDead;
	
	UPROPERTY(BlueprintAssignable)
	FHealthDamagedSignature OnHealthDamaged;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintPure)
	float GetHealth() const { return CurrentHealth; }
	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const { return MaxHealth; }
	
protected:
	virtual void BeginPlay() override;
	
	// MaxHealth, CurrentHealth는 어떤 프로젝트든 아래와 동일
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentHealth;

private:
	// 내 함수로 TakeDamage를 바인딩해올 수 있음
	// -> 시그니처를 맞춰주자
	// -> 언리얼에서 만든 기능을 바인딩할거니까 UFUNCTION() 붙여주기
	
	UFUNCTION()
	void DamageTake(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* Causer);

};
