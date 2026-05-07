// WeaponSandboxBase.cpp

#include "WeaponSandboxBase.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"


void AWeaponSandboxBase::Fire()
{
	SandboxFire();					// 부모꺼 내용 아예 버리고, 새로운 걸로 덮어쓰기
									// 전혀 다른 종류들의 총을 기획하기 위해 기존걸 버림
}

void AWeaponSandboxBase::Reload()
{
	CurrentAmmo = MaxAmmo;
	HandleFireDelay();				// 리로드 하면 바로 총 쏠 수 있게
}

bool AWeaponSandboxBase::CheckAmmo()
{
	return AmmoPerFire <= CurrentAmmo;
}

void AWeaponSandboxBase::LinetraceOneShot(FVector Direction)
{
	// ForceInit : 구조체를 강제로 초기화할 수 있음
	FHitResult Hit(ForceInit);
	
	FVector Start = FirePoint->GetComponentLocation();
	FVector End = Start + (Direction * Range);
	
	if (!UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		Start, End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false, 
		{this, GetOwner()},
		EDrawDebugTrace::ForDuration,
		Hit,
		true,
		FLinearColor::Red, FLinearColor::Green, 5
	)) return;
	
	UGameplayStatics::ApplyDamage(
	Hit.GetActor(),              // 데미지 받을 대상 액터
	DamagePerHit,                // 데미지 양
	GetInstigatorController(),   // 데미지 유발자 (컨트롤러)
	GetOwner(),                  // 데미지 발생시킨 액터 (무기)
	UDamageType::StaticClass()   // 데미지 타입
);
}

void AWeaponSandboxBase::PlaySound(USoundBase* Sound)
{
	UGameplayStatics::PlaySoundAtLocation(this, Sound,GetActorLocation());
}

void AWeaponSandboxBase::UpdateAmmo()
{
	CurrentAmmo -= AmmoPerFire;
}
