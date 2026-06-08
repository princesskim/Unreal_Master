// MyTorchlight.cpp

#include "InterfaceTest/MyTorchlight.h"
#include "InterfaceTest/TestMyInterface.h"
#include "Kismet/KismetSystemLibrary.h"

AMyTorchlight::AMyTorchlight()
{
 	PrimaryActorTick.bCanEverTick = true;
}

void AMyTorchlight::BeginPlay()
{
	Super::BeginPlay();
	
	// 인터페이스가 있는 액터면 불붙이라는 신호를 보냄
	for (const TWeakObjectPtr<AActor>& Item : Items)
	{
		// 해당 클래스에 인터페이스가 구현이 되어 있는지 물어보기
		// 두번째 인자로 U인터페이스 타입을 받음
		if (UKismetSystemLibrary::DoesImplementInterface(Item.Get(), UTestMyInterface::StaticClass()))
		{
			// 함수명 앞에 Execute_ 붙여주기
			ITestMyInterface::Execute_OnFireDetected(Item.Get(), 100.f, FVector::ZeroVector);
		}
	}
}

void AMyTorchlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

