// TestActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

UCLASS()
class UNREALMASTER_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATestActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, Category="Setting")
	TSoftObjectPtr<UStaticMesh> MySoftMesh;
	
	void LoadWithSoftPtr();
	
private:
	// 비동기 로드 방식
	// 서브 스레드에서 로딩 완료되면, 나중에 실행될 함수
	void OnLoadCompleted();
};
