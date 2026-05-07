// TraceTest.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TraceTest.generated.h"

UCLASS()
class UNREALMASTER_API ATraceTest : public AActor
{
	GENERATED_BODY()
	
public:	
	ATraceTest();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void StartAsyncTrace();
	void OnAsyncTraceComplete(const FTraceHandle& Handle, FTraceDatum& Data);
};
