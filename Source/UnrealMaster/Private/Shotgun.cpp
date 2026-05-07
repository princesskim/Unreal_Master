// Shotgun.cpp

#include "Shotgun.h"
#include "GunStatComponent.h"

AShotgun::AShotgun()
{
	GunStat->BulletsPerShot = 8;
	GunStat->SpreadAngle = 15.f;
	GunStat->Damage = 15.f;
	GunStat->MaxAmmo = 24;
	GunStat->CurrentAmmo = GunStat->MaxAmmo;
	GunStat->FireRate = 0.8f;
	GunStat->RecoilPitch = 2.f;
	GunStat->RecoilYaw = 0.5f;
}
