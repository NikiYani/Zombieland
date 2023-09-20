// Zombieland by NikiYani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZLDefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UZLHealthComponent;
class UZLStaminaComponent;
class UAnimMontage;

UCLASS()
class ZOMBIELAND_API AZLDefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZLDefaultCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	UZLHealthComponent* GetHealthComponent() const { return HealthComponent; } 

	UFUNCTION()
	bool GetSprint() const { return isSprint; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	UZLHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Stamina")
	UZLStaminaComponent* StaminaComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	bool isSprint = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	float CurrMaxWalkSpeed = 600.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	float CurrMinWalkSpeed = 300.0f;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Zoom(float Value);
	void RotationPlayerOnCursor();
	void OnHealthChanged(float NewHealth);
	void OnStaminaChanged(float NewStamina);
	void OnDeath();
	void Sprint();
	void Jog();
	void DecreaseStamina();
	void IncreaseStamina();

	FTimerHandle StaminaDecreaseTimer;
	FTimerHandle StaminaIncreaseTimer;

	float YRotation = -75.0f;
	float ArmLengthMax = 1400.0f;
	float ArmLengthMin = 900.0f;
	float FOV = 55.0f;
};
