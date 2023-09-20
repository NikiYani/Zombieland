// Zombieland by NikiYani. All Rights Reserved.


#include "Components/ZLStaminaComponent.h"
#include "Components/ZLHealthComponent.h"


// Sets default values for this component's properties
UZLStaminaComponent::UZLStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

bool UZLStaminaComponent::IsFatigue() const
{
	return Stamina == 0.0f;
}

bool UZLStaminaComponent::IsFullStamina() const
{
	return Stamina == MaxStamina;
}

bool UZLStaminaComponent::ChangeStamina(float NewStamina)
{
	Stamina = FMath::Clamp(Stamina + NewStamina, 0.0f, MaxStamina);
	OnStaminaChanged.Broadcast(Stamina);
	return true;
}

// Called when the game starts
void UZLStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
	Stamina = MaxStamina;

	OnStaminaChanged.Broadcast(Stamina);
}