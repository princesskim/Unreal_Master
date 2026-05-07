// MasterPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MasterPlayerController.generated.h"

class ABaseGun;
class UInputMappingContext;
class UInputAction;

UCLASS()
class UNREALMASTER_API AMasterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMasterPlayerController();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ReloadAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	ABaseGun* CurrentGun;
	
protected:
	virtual void BeginPlay() override;
};
