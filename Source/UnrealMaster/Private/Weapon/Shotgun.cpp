// Shotgun.cpp

#include "Weapon/Shotgun.h"
#include "Weapon/GunStatComponent.h"

AShotgun::AShotgun()
{
	GunStat->BulletsPerFire = 8;
	GunStat->SpreadAngle = 15.f;
	GunStat->Damage = 15.f;
	GunStat->MaxAmmo = 24;
	GunStat->CurrentAmmo = GunStat->MaxAmmo;
	GunStat->FireRate = 0.8f;
	GunStat->RecoilPitch = 2.f;
	GunStat->RecoilYaw = 0.5f;
}

bool AShotgun::SandboxFire_Implementation()
{
	if (!CheckAmmo()) return false;
	if (!bCanFire) return false;
	if (bIsReloading) return false;
	
	bCanFire = false;
	for (int32 i = 0; i < GunStat->BulletsPerFire; ++i)
	{
		FVector Dir = FMath::VRandCone(GetActorForwardVector(), FMath::DegreesToRadians(GunStat->SpreadAngle));
		
		LinetraceOneShot(Dir);
	}
	UpdateAmmo();
	
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow,
		FString::Printf(TEXT("[%s] Fire! %d Bullets | Ammo: %d / %d"),
			*GetName(), GunStat->BulletsPerFire, GunStat->CurrentAmmo, GunStat->MaxAmmo));
	
	
	GetWorldTimerManager().SetTimer(
		FireRateCooldownHandle, this,
		&AShotgun::ResetFireCooldown,
		GunStat->FireRate, false
		);
	
	return true;
}
