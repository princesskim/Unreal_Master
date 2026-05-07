// WeaponTemplateBase.cpp

#include "WeaponTemplateBase.h"

void AWeaponTemplateBase::Fire()
{
	if(!CanFire) return;
	
	if(CheckAmmo())
	{
		// 순서를 정하자!
		PlayEffects();
		ProcessFiring();
		UpdateAmmo();
		
		// 부모꺼에서 FireDelay 타이머 부르는 거 그대로 이용
		Super::Fire();
		return;
	}
	
	Reload();						// 탄약 부족하면 자동으로 재장전
}

void AWeaponTemplateBase::Reload_Implementation()
{
	CurrentAmmo = MaxAmmo;
	HandleFireDelay();				// 리로드 하면 바로 총 쏠 수 있게
}

bool AWeaponTemplateBase::CheckAmmo_Implementation()
{
	// 쏠 수 있는지를 리턴
	return AmmoPerFire <= CurrentAmmo;
}

void AWeaponTemplateBase::UpdateAmmo_Implementation()
{
	CurrentAmmo -= AmmoPerFire;
}
