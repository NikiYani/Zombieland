// Zombieland by NikiYani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZLPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIELAND_API AZLPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AZLPlayerController();

protected:
	virtual void BeginPlay() override;
	
};
