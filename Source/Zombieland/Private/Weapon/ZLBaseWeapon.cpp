// Zombieland by NikiYani. All Rights Reserved.


#include "Weapon/ZLBaseWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

// Sets default values
AZLBaseWeapon::AZLBaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponComponent);

	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
}

bool AZLBaseWeapon::IsCurrentClipEmpty() const
{
	return CurrentAmmoWeapon.Bullets == 0;
}

bool AZLBaseWeapon::IsClipFull() const
{
	return CurrentAmmoWeapon.Bullets == AmmoWeapon.Bullets;
}

// Called every frame
void AZLBaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZLBaseWeapon::Fire() 
{
	Shoot();
}

void AZLBaseWeapon::DecrementBullets()
{
	CurrentAmmoWeapon.Bullets--;
	UE_LOG(LogWeapon, Display, TEXT("Bullets = %s"), *FString::FromInt(CurrentAmmoWeapon.Bullets));
	OnBulletsChanged.Broadcast(CurrentAmmoWeapon.Bullets);

	if (IsCurrentClipEmpty())
	{
		OnReload.Broadcast();
	}
}

void AZLBaseWeapon::ChangeClip()
{
	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
	OnBulletsChanged.Broadcast(CurrentAmmoWeapon.Bullets);
}

// Called when the game starts or when spawned
void AZLBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AZLBaseWeapon::Shoot() 
{
	const FTransform SocketTransform = WeaponComponent->GetSocketTransform("Muzzle");
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);

	FVector TracerEnd = TraceEnd;
	if (HitResult.bBlockingHit)
	{
		TracerEnd = HitResult.ImpactPoint;
	}
	SpawnTrace(TraceStart, TracerEnd);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootWave, TraceStart);

	DecrementBullets();
}

void AZLBaseWeapon::SpawnTrace(const FVector& TraceStart, const FVector& TraceEnd) 
{
	const auto TraceFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceEffect, TraceStart);
	if (TraceFX)
	{
		TraceFX->SetNiagaraVariableVec3(TraceName, TraceEnd);
	}
}

