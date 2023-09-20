// Zombieland by NikiYani. All Rights Reserved.


#include "Components/ZLHealthComponent.h"

// Sets default values for this component's properties
UZLHealthComponent::UZLHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UZLHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	OnHealthChanged.Broadcast(Health);

	AActor* OwnerComponent = GetOwner();
	if (OwnerComponent)
	{
		OwnerComponent->OnTakeAnyDamage.AddDynamic(this, &UZLHealthComponent::OnTakeAnyDamage);
	}
}

bool UZLHealthComponent::IsDead() const
{
	return Health <= 0.0f;
}

void UZLHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsDead())
		return;
	
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}

bool UZLHealthComponent::AddHealth(float NewHealth)
{
	if (IsDead() || IsHealthFull())
		return false;

	Health = FMath::Clamp(Health + NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
	return true;
}

bool UZLHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}
