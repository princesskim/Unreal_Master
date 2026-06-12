// PooledObject.cpp

#include "ObjectPooling/PooledObject.h"
#include "ObjectPooling/MyObjectPool.h"

void UPooledObject::Init(class AMyObjectPool* Owner)
{
	bIsPoolActive = false;
	ObjectPool = Owner;
}

void UPooledObject::RecycleSelf()
{
	ObjectPool->RecyclePooledObject(this);
}

void UPooledObject::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	// 바인드 풀기
	ObjectPool->OnPoolerCleanup.RemoveDynamic(this, &UPooledObject::RecycleSelf);
	
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}