// PooledObject_Ver2.cpp

#include "ObjectPooling_Ver2/PooledObject_Ver2.h"
#include "ObjectPooling_Ver2/ObjectPoolSubsystem.h"


void UPooledObject_Ver2::Init()
{
	bIsPoolActive = false;
	PoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
}

void UPooledObject_Ver2::RecycleSelf()
{
	if (PoolSubsystem)
	{
		//PoolSubsystem->RecyclePooledObject(this);
	}
}

void UPooledObject_Ver2::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (PoolSubsystem)
	{
		// 바인드 풀기
		//PoolSubsystem->OnPoolerCleanup_Ver2.RemoveDynamic(this, &UPooledObject_Ver2::RecycleSelf);
	}
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}
