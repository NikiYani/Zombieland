// Zombieland by NikiYani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZLHealthPickup.generated.h"

class USphereComponent;
class AZLDefaultCharacter;

UCLASS()
class ZOMBIELAND_API AZLHealthPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZLHealthPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float RespawnTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "5.0", ClampMax = "100.0"))
	float HealthFromPickup = 100.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool GivePickup(AZLDefaultCharacter* Character);
	void PickupWasTaken();
	void RespawnPickup();

};
