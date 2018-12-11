// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class TESTPROJECT_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ATestProjectProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

	// Mag size
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		int MagSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FireRate)
		int reloadTime;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
		TSubclassOf<class UCameraShake> FireCamShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
		UParticleSystem* BeamEffect;

	void CastRay(AActor* MyOwner);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	USkeletalMeshComponent* GetWeaponMesh() { return MeshComp; };

	bool FireWeapon();

	// Compiling the function Reload() of WeaponBase class, which resets mag size to what it was
	void Reload();

	bool Empty = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FireRate)
		int reloadCurrValue;
	
	int Damage = 35;

	TSubclassOf<UDamageType> DamageType;

};
