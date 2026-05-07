// BaseGun.cpp

#include "BaseGun.h"
#include "GunStatComponent.h"
#include "Kismet//KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

ABaseGun::ABaseGun()
{
 	PrimaryActorTick.bCanEverTick = false;
	GunStat = CreateDefaultSubobject<UGunStatComponent>(TEXT("GunStat"));
	bCanFire = true;
	bIsReloading = false;
	
	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	RootComponent = GunMesh;
}

bool ABaseGun::Fire()
{
	if (GunStat->CurrentAmmo <= 0) return false;
	if (!bCanFire) return false;
	if (bIsReloading) return false;
	
	bCanFire = false;
	for (int32 i = 0; i < GunStat->BulletsPerShot; ++i)
	{
		FVector Dir = FMath::VRandCone(GetActorForwardVector(), FMath::DegreesToRadians(GunStat->SpreadAngle));
		FVector End = GetActorLocation() + Dir * GunStat->Range;
		FireTrace(End);
	}
	GunStat->CurrentAmmo = FMath::Clamp(GunStat->CurrentAmmo - GunStat->BulletsPerShot, 0, GunStat->MaxAmmo);
	
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow,
		FString::Printf(TEXT("[%s] Fire! %d Bullets | Ammo: %d / %d"),
			*GetName(), GunStat->BulletsPerShot, GunStat->CurrentAmmo, GunStat->MaxAmmo));
	
	
	GetWorldTimerManager().SetTimer(
		FireRateCooldownHandle, this,
		&ABaseGun::ResetFireCooldown,
		GunStat->FireRate, false
		);
	
	return true;
}

void ABaseGun::FireTrace(FVector End)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	FHitResult HitResult;
	
	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		GetActorLocation(),
		End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green
	);
	
	if (HitResult.bBlockingHit)
	{
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 8, FColor::Red, false, 2.f);
		
		FVector Start = GetActorLocation();

		if (HitResult.bBlockingHit)
		{
			DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Red, false, 2.f);
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);
		}
		
		// 디버그 메시지 추가
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
			FString::Printf(TEXT("Hit: %s"), 
				HitResult.GetActor() ? *HitResult.GetActor()->GetName() : TEXT("None")));
	}
	
	if (HitResult.GetActor())
	{
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 8, FColor::Red, false, 2.f);
		
		UGameplayStatics::ApplyPointDamage(
			HitResult.GetActor(),		// 데미지 받을 액터
			GunStat->Damage,			// 데미지 수치
			(End - GetActorLocation()).GetSafeNormal(),
			HitResult,
			nullptr,
			this,
			nullptr
		);
	}
	
}

void ABaseGun::Reload()
{
	if (GunStat->CurrentAmmo >= GunStat->MaxAmmo) return;
	if (bIsReloading) return;
	bIsReloading = true;
	
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow,
		FString::Printf(TEXT("[%s] Reloading..."), *GetName()));
	
	GetWorldTimerManager().SetTimer(
		ReloadTimerHandle, this,
		&ABaseGun::OnReloadComplete,
		GunStat->ReloadTime, false
		);
}

void ABaseGun::ResetFireCooldown()
{
	bCanFire = true;
}

void ABaseGun::OnReloadComplete()
{
	GunStat->CurrentAmmo = GunStat->MaxAmmo;
	bIsReloading = false;
	
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green,
		FString::Printf(TEXT("[%s] Reload Complete! Ammo: %d / %d"),
			*GetName(), GunStat->CurrentAmmo, GunStat->MaxAmmo));
}
