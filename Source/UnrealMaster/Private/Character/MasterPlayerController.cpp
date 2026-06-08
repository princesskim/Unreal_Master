// MasterPlayerController.cpp

#include "Character/MasterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "UI/PlayerStatusWidget.h"
#include "Character/UnrealMasterCharacter.h"

AMasterPlayerController::AMasterPlayerController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr), 
	JumpAction(nullptr), 
	LookAction(nullptr), 
	SprintAction(nullptr),
	FireAction(nullptr),
	ReloadAction(nullptr),
	AimAction(nullptr)
{
}

void AMasterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{	
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = 
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{	
			if (InputMappingContext)
			{
				SubSystem->AddMappingContext(InputMappingContext, 0);
			}
			
		}
	}
	
	if (PlayerStatusWidgetClass)
	{
		PlayerStatusWidget = CreateWidget<UPlayerStatusWidget>(this, PlayerStatusWidgetClass);
		if (PlayerStatusWidget)
		{
			PlayerStatusWidget->AddToViewport();
			
			APawn* MyPawn = GetPawn();
			if (MyPawn)
			{
				AUnrealMasterCharacter* UnrealMasterCharacter = Cast<AUnrealMasterCharacter>(MyPawn);
				if (UnrealMasterCharacter)
				{
					PlayerStatusWidget->SetHealthComp(UnrealMasterCharacter->GetHealthComponent());
				}
			}
		}
	}
}