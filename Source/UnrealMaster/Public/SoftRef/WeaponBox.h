// WeaponBox.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBox.generated.h"

UCLASS()
class UNREALMASTER_API AWeaponBox : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBox();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* WeaponTable;
	
	void OpenBox();

private:
	
	// 비동기 로드가 완료되면 실행될 함수
	UFUNCTION()
	void OnWeaponAsyncSpawned(TSoftClassPtr<AActor> WeaponClassPtr);
};
