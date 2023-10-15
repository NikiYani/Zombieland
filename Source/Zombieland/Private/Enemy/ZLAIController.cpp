// Zombieland by NikiYani. All Rights Reserved.


#include "Enemy/ZLAIController.h"
#include "Enemy/ZLEnemyCharacter.h"

void AZLAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto AICharacter = Cast<AZLEnemyCharacter>(InPawn);
	if (AICharacter)
	{
		RunBehaviorTree(AICharacter->BehaviorTreeAsset);
	}
}