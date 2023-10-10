// Zombieland by NikiYani. All Rights Reserved.


#include "Components/ZLWeaponComponent.h"

#include "GameFramework/Actor.h"
#include "Weapon/ZLBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animation/ZLReloadFinishedAnimNotify.h"
#include "Weapon/ZLBaseWeapon.h"


// Sets default values for this component's properties
UZLWeaponComponent::UZLWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UZLWeaponComponent::Fire() 
{
	if (Weapon && !AnimReloading)
	{
		Weapon->Fire();
		const auto Character = Cast<ACharacter>(GetOwner());
		Character->GetWorldTimerManager().SetTimer(FireTimer, Weapon, &AZLBaseWeapon::Fire, 0.3f, true);
	}
}

void UZLWeaponComponent::StopFire() 
{
	const auto Character = Cast<ACharacter>(GetOwner());
	Character->GetWorldTimerManager().ClearTimer(FireTimer);
}

// Called when the game starts
void UZLWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();	
	InitAnimNotify();
}

bool UZLWeaponComponent::GetCurrentWeaponAmmo(FAmmoWeapon& AmmoWeapon) const
{
	if (Weapon)
	{
		AmmoWeapon = Weapon->GetCurrentAmmoWeapon();
		return true;
	}
	return false;
}


void UZLWeaponComponent::SpawnWeapon() 
{
	Weapon = GetWorld()->SpawnActor<AZLBaseWeapon>(WeaponClass);
	if (Weapon)
	{
		const auto Character = Cast<ACharacter>(GetOwner());
		if (Character)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "r_Weapon_Socket");

			OnBulletsChanged(Weapon->GetBullets());

			Weapon->OnBulletsChanged.AddUObject(this, &UZLWeaponComponent::OnBulletsChanged);
			Weapon->OnReload.AddUObject(this, &UZLWeaponComponent::OnReload);
		}
	}
}

void UZLWeaponComponent::Reload() 
{
	if (!CanReload())
		return;
	StopFire(); 
	Weapon->ChangeClip();
	AnimReloading = true;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	Character->PlayAnimMontage(ReloadMontage);
}

void UZLWeaponComponent::OnReload() 
{
	Reload();
}

void UZLWeaponComponent::InitAnimNotify()
{
	if (!ReloadMontage)
		return;

	const auto NotifiesEvents = ReloadMontage->Notifies;
	for (auto NotifyEvent : NotifiesEvents)
	{
		auto ReloadFinish = Cast<UZLReloadFinishedAnimNotify>(NotifyEvent.Notify);
		if (ReloadFinish)
		{
			ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &UZLWeaponComponent::OnNotifyReloadFinished);
			break;
		}
	}
}

void UZLWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh)
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character->GetMesh() == SkeletalMesh)
	{
		AnimReloading = false;
	}
}

bool UZLWeaponComponent::CanReload() const
{
	return !AnimReloading && !Weapon->IsClipFull();
}

void UZLWeaponComponent::OnBulletsChanged(int32 NewBullets) 
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Bullets = %d"), NewBullets));
}

// Called every frame
void UZLWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
