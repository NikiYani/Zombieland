// Zombieland by NikiYani. All Rights Reserved.


#include "Enemy/ZLEnemyCharacter.h"
#include "Components/ZLHealthComponent.h"

// Sets default values
AZLEnemyCharacter::AZLEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<UZLHealthComponent>("HealthComponent");
}

// Called every frame
void AZLEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void AZLEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}