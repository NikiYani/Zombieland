// Zombieland by NikiYani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZLStaminaComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaminaChanged, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIELAND_API UZLStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UZLStaminaComponent();

	UFUNCTION(BlueprintCallable)
	float GetStamina() const { return Stamina; }
	
	UFUNCTION(BlueprintCallable)
	bool IsFatigue() const;

	UFUNCTION(BlueprintCallable)
	bool IsFullStamina() const;

	bool ChangeStamina(float NewStamina);
	
	FOnStaminaChanged OnStaminaChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxStamina = 100.0f;

private:	
	float Stamina = 0.0f;
};
