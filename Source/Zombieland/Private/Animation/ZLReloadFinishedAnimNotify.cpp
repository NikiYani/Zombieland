// Zombieland by NikiYani. All Rights Reserved.


#include "Animation/ZLReloadFinishedAnimNotify.h"

void UZLReloadFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotifyReloadFinished.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}