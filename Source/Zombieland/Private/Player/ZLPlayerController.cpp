// Zombieland by NikiYani. All Rights Reserved.


#include "Player/ZLPlayerController.h"

AZLPlayerController::AZLPlayerController() 
{
}

void AZLPlayerController::BeginPlay() 
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}