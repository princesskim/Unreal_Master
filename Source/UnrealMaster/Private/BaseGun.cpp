// BaseGun.cpp

#include "BaseGun.h"
#include "GunStatComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet//KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

ABaseGun::ABaseGun()
{
 	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(RootComponent);
	
	FirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(RootComponent);
	
	
	GunStat = CreateDefaultSubobject<UGunStatComponent>(TEXT("GunStat"));
	
	bCanFire = true;
	bIsReloading = false;
}

bool ABaseGun::Fire()
{
	return SandboxFire();
}

bool ABaseGun::SandboxFire_Implementation()
{
	return false;  // 기본 구현은 false
}

void ABaseGun::LinetraceOneShot(FVector Direction)
{
	FVector Start = FirePoint->GetComponentLocation();
	FVector End = Start + (Direction * GunStat->Range);
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	FHitResult HitResult;
	
	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		0.2f
	);
	
	// if (HitResult.bBlockingHit)
	// {
	// 	// 디버그 메시지
	// 	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
	// 		FString::Printf(TEXT("Hit: %s"), 
	// 			HitResult.GetActor() ? *HitResult.GetActor()->GetName() : TEXT("None")));
	// }
	
	if (HitResult.GetActor())
	{
		UGameplayStatics::ApplyPointDamage(
			HitResult.GetActor(),		// 데미지 받을 액터
			GunStat->Damage,			// 데미지 수치
			(End - Start).GetSafeNormal(),
			HitResult,
			nullptr,
			this,
			nullptr
		);
	}
	
}

void ABaseGun::PlaySound(USoundBase* Sound)
{
	UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation());
}

bool ABaseGun::CheckAmmo()
{
	return GunStat->BulletsPerFire <= GunStat->CurrentAmmo;
}

void ABaseGun::UpdateAmmo()
{
	GunStat->CurrentAmmo -= GunStat->BulletsPerFire;
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
