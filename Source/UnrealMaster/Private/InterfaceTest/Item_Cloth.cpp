// Item_Cloth.cpp

#include "InterfaceTest/Item_Cloth.h"
#include "Kismet/GameplayStatics.h"			// 스폰 함수
#include "Particles/ParticleSystem.h"		// 파티클

void AItem_Cloth::OnFireDetected_Implementation(float Temperature, FVector HitLocation)
{
	if (FireEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			FireEffect,
			GetActorLocation(),
			GetActorRotation(),
			FVector(1.f)
		);
		
	}
}