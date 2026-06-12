// PooledObject_Ver2.cpp

#include "ObjectPooling_Ver2/PooledObject_Ver2.h"

void UPooledObject_Ver2::Init()
{
	bIsPoolActive = false;
}

void UPooledObject_Ver2::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}
