// WeaponBase.cpp

#include "WeaponBase.h"
#include "Components/ArrowComponent.h"


AWeaponBase::AWeaponBase()
{
 	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	FirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(RootComponent);

	AmmoPerFire = 1;
	CurrentAmmo = 0;
	MaxAmmo = 12;
	RoF = 1.f;
	CanFire = true;
	Range = 1000.f;
	DamagePerHit = 30.f;

}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmo;
}

void AWeaponBase::Fire()
{
	CanFire = false;
	GetWorldTimerManager().SetTimer(
		TimerFireDelay, this,
		&AWeaponBase::HandleFireDelay,
		1.f/RoF, false
	);
}

void AWeaponBase::HandleFireDelay()
{
	GetWorldTimerManager().ClearTimer(TimerFireDelay);
	CanFire = true;
}
