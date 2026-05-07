// EnemyCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class UNREALMASTER_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float SightAngle;   // 시야각 (도)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float SightRange;   // 시야 거리

public:	
	void StartAsyncTrace();
	void OnAsyncTraceComplete(const FTraceHandle& Handle, FTraceDatum& Data);
	
private:
	FTraceDelegate TraceDelegate;
	
	FTimerHandle SightTraceTimerHandle;
	float SightTraceInterval;
};
