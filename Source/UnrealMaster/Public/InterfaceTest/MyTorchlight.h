// MyTorchlight.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyTorchlight.generated.h"

UCLASS()
class UNREALMASTER_API AMyTorchlight : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyTorchlight();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	// 계속 가지고 다닐 액터들이 아니기 때문에 TWeakObjectPtr
	UPROPERTY(EditAnywhere)
	TArray<TWeakObjectPtr<AActor>> Items;
};
