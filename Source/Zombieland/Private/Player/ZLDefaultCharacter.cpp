// Zombieland by NikiYani. All Rights Reserved.


#include "Player/ZLDefaultCharacter.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/ZLHealthComponent.h"
#include "Components/ZLStaminaComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "Components/ZLStaminaComponent.h"
#include "Components/ZLWeaponComponent.h"


// Sets default values
AZLDefaultCharacter::AZLDefaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());

	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = ArmLengthMax;
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	HealthComponent = CreateDefaultSubobject<UZLHealthComponent>("HealthComponent");
	StaminaComponent = CreateDefaultSubobject<UZLStaminaComponent>("StaminaComponent");

	WeaponComponent = CreateDefaultSubobject<UZLWeaponComponent>("Weapon");
}

// Called every frame
void AZLDefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!(HealthComponent->IsDead()))
	{
		RotationPlayerOnCursor();
	}
}

// Called to bind functionality to input
void AZLDefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AZLDefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AZLDefaultCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Zoom", this, &AZLDefaultCharacter::Zoom);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AZLDefaultCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AZLDefaultCharacter::Jog);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UZLWeaponComponent::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UZLWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UZLWeaponComponent::Reload);
}

// Called when the game starts or when spawned
void AZLDefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &AZLDefaultCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &AZLDefaultCharacter::OnHealthChanged);

	OnStaminaChanged(StaminaComponent->GetStamina());
	StaminaComponent->OnStaminaChanged.AddUObject(this, &AZLDefaultCharacter::OnStaminaChanged);
}

void AZLDefaultCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AZLDefaultCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void AZLDefaultCharacter::Zoom(float Value)
{
	float& armLen = SpringArmComponent->TargetArmLength;

	if (armLen + Value <= ArmLengthMax && armLen + Value >= ArmLengthMin)
		armLen += Value;
}

void AZLDefaultCharacter::RotationPlayerOnCursor()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}
}

void AZLDefaultCharacter::OnHealthChanged(float NewHealth) 
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Health = %f"), NewHealth));
}

void AZLDefaultCharacter::OnStaminaChanged(float NewStamina) 
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Stamina = %f"), NewStamina));
}

void AZLDefaultCharacter::OnDeath() 
{
	CurrentCursor->DestroyRenderState_Concurrent();
	PlayAnimMontage(DeathMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

void AZLDefaultCharacter::Sprint() 
{
	isSprint = true;

	GetWorldTimerManager().SetTimer(StaminaDecreaseTimer, this, &AZLDefaultCharacter::DecreaseStamina, 1.0f, true);
	GetWorldTimerManager().ClearTimer(StaminaIncreaseTimer);
	GetCharacterMovement()->MaxWalkSpeed = CurrMaxWalkSpeed;
}

void AZLDefaultCharacter::Jog()
{
	isSprint = false;

	GetCharacterMovement()->MaxWalkSpeed = CurrMinWalkSpeed;
	GetWorldTimerManager().ClearTimer(StaminaDecreaseTimer);
	GetWorldTimerManager().SetTimer(StaminaIncreaseTimer, this, &AZLDefaultCharacter::IncreaseStamina, 1.0f, true);
}

void AZLDefaultCharacter::DecreaseStamina() 
{
	if (StaminaComponent->IsFatigue())
	{
		GetWorldTimerManager().ClearTimer(StaminaDecreaseTimer);
		isSprint = false;
		GetCharacterMovement()->MaxWalkSpeed = CurrMinWalkSpeed;
		return;
	}

	StaminaComponent->ChangeStamina(-20.0f);
}

void AZLDefaultCharacter::IncreaseStamina() 
{
	if (StaminaComponent->IsFullStamina())
	{
		GetWorldTimerManager().ClearTimer(StaminaIncreaseTimer);
		return;
	}

	StaminaComponent->ChangeStamina(10.0f);
}