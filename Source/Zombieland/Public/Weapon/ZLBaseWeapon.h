// Zombieland by NikiYani. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZLBaseWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnReload)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBulletsChanged, int32);

USTRUCT(BlueprintType)
struct FAmmoWeapon
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Clips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool Infinite;
};

UCLASS()
class ZOMBIELAND_API AZLBaseWeapon : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AZLBaseWeapon();

	bool IsCurrentClipEmpty() const;
	bool IsClipFull() const;

	int32 GetBullets() const { return CurrentAmmoWeapon.Bullets; }

	virtual void Tick(float DeltaTime) override;

	void Fire();
	void DecrementBullets();
	void ChangeClip();

	FOnReload OnReload;
	FOnBulletsChanged OnBulletsChanged;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Shoot();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceDistance = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoWeapon AmmoWeapon{30, 0, true};

	FAmmoWeapon CurrentAmmoWeapon;
};
