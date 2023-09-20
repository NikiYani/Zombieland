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

void AZLPlayerController::BeginSpectatingState() 
{
	SetControlRotation(FRotator(-75.0f, 0.0f, 0.0f));
	Super::BeginSpectatingState();
}
