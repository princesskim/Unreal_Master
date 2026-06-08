// PlayerStatusWidget.cpp

#include "UI/PlayerStatusWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Character/MyActorComponent.h"

void UPlayerStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerStatusWidget::OnHealthDamaged(float NewHealth, float MaxHealth, float HealthChange)
{
	UpdateHealthUI(NewHealth, MaxHealth);
}

void UPlayerStatusWidget::UpdateHealthUI(float CurrentHealth, float MaxHealth)
{
	float Percent = (MaxHealth > 0.0f) ? CurrentHealth / MaxHealth : 0.f;
	int32 CurrentHealthInt = FMath::RoundToInt(CurrentHealth);
	int32 MaxHealthInt = FMath::RoundToInt(MaxHealth);
	FString HealthString = FString::Printf(TEXT("%d / %d"), CurrentHealthInt, MaxHealthInt);
	
	if (HealthBar) HealthBar->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));
	if (HealthText) HealthText->SetText(FText::FromString(HealthString));
}

void UPlayerStatusWidget::SetHealthComp(UMyActorComponent* InHealthComp)
{
	if (!InHealthComp) return;
	HealthComp = InHealthComp;
	
	// NativeConstruct 시점에는 아직 HealthComp가 없을 수 있기 때문에 여기서 바인드 하기
	HealthComp->OnHealthDamaged.AddUniqueDynamic(this, &UPlayerStatusWidget::OnHealthDamaged);
	
	float CurrentHealth = HealthComp->GetHealth();
	float MaxHealth = HealthComp->GetMaxHealth();
	
	UpdateHealthUI(CurrentHealth, MaxHealth);
	/*UE_LOG(
		LogTemp,
		Warning,
		TEXT("SetHealthComp Owner=%s Health=%f"),
		*HealthComp->GetOwner()->GetName(),
		HealthComp->GetHealth()
	);*/
}
