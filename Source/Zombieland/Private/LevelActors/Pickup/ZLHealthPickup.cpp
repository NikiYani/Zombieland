// Zombieland by NikiYani. All Rights Reserved.


#include "LevelActors/Pickup/ZLHealthPickup.h"
#include "Components/SphereComponent.h"
#include "Player/ZLDefaultCharacter.h"
#include "Components/ZLHealthComponent.h"

// Sets default values
AZLHealthPickup::AZLHealthPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(SphereComponent);
}

// Called when the game starts or when spawned
void AZLHealthPickup::BeginPlay()
{
	Super::BeginPlay();
}

void AZLHealthPickup::NotifyActorBeginOverlap(AActor* OtherActor) 
{
	Super::NotifyActorBeginOverlap(OtherActor);
	const auto Character = Cast<AZLDefaultCharacter>(OtherActor);
	if (GivePickup(Character))
	{
		PickupWasTaken();
	}
}

// Called every frame
void AZLHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AZLHealthPickup::GivePickup(AZLDefaultCharacter* Character)
{
	const auto HealthComponent = Character->GetHealthComponent();
	if (!HealthComponent)
		return false;

	return HealthComponent->AddHealth(HealthFromPickup);
}

void AZLHealthPickup::PickupWasTaken()
{
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetRootComponent()->SetVisibility(false, true);

	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AZLHealthPickup::RespawnPickup, RespawnTime);
}

void AZLHealthPickup::RespawnPickup()
{
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	GetRootComponent()->SetVisibility(true, true);
}

