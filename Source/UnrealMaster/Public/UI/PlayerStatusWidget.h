// PlayerStatusWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatusWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UMyActorComponent;

UCLASS()
class UNREALMASTER_API UPlayerStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	virtual void NativeConstruct() override;
	
	// BindWidget : C++의 HealthBar와 블루프린트의 HealthBar를 연결해주는 기능
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;
	
	UPROPERTY()
	UMyActorComponent* HealthComp;
	
	UFUNCTION()
	void OnHealthDamaged(float NewHealth, float MaxHealth, float HealthChange);
	
	void UpdateHealthUI(float CurrentHealth, float MaxHealth);
	
public:
	void SetHealthComp(UMyActorComponent* InHealthComp);
};
