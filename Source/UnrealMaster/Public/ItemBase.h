// ItemBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class UMyActorComponent;

UCLASS()
class UNREALMASTER_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemBase();

	UFUNCTION()					// 바인딩해줘야 하니까 리플렉션 등록
	void OnPlayerDead(AController* InstigatorController);
								// AItemBase는 AActor를 상속받음 → 이미 Instigator 멤버가 존재 
								// Instigator를 파라미터 이름으로 쓰면 섀도잉 에러가 남
								// Instigator 대신 InstigatorController 같은 걸로 하기
	
protected:
	virtual void BeginPlay() override;
	
	UMyActorComponent* HealthComponent;

public:	
	virtual void Tick(float DeltaTime) override;
	
};
