// GunStatComponent.cpp

#include "GunStatComponent.h"

UGunStatComponent::UGunStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MaxAmmo = 30;
	CurrentAmmo = 30;
	BulletsPerShot = 1;
	SpreadAngle = FMath::DegreesToRadians(0.f);
	Range = 1000.f;
	Damage = 10.f;
	FireRate = 0.1f;
	ReloadTime = 2.f;
}