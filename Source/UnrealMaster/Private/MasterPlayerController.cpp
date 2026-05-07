// MasterPlayerController.cpp

#include "MasterPlayerController.h"
#include "EnhancedInputSubsystems.h"

AMasterPlayerController::AMasterPlayerController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr), 
	JumpAction(nullptr), 
	LookAction(nullptr), 
	SprintAction(nullptr),
	FireAction(nullptr),
	ReloadAction(nullptr)
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
}