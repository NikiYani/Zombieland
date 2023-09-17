// Zombieland by NikiYani. All Rights Reserved.


#include "ZLGameMode.h"
#include "Player/ZLDefaultCharacter.h"
#include "Player/ZLPlayerController.h"


AZLGameMode::AZLGameMode()
{
	DefaultPawnClass = AZLDefaultCharacter::StaticClass();
	PlayerControllerClass = AZLPlayerController::StaticClass();
}