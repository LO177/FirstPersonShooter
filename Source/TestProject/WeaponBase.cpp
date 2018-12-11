// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "TestProjectProjectile.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "NPCBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(MeshComp);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Set mag size
	MagSize = 10;
	
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (reloadCurrValue > 0) {
		reloadCurrValue--;
	}

}

bool AWeaponBase::FireWeapon() {
	//weaponFired = true;

	UWorld* const World = GetWorld();
	if (World != NULL)
	{
			//const FRotator SpawnRotation = GetControlRotation();
			//FRotator SpawnRotation;
			//if (customFramework) 
			//{
			//	//SpawnRotation = GetControlRotation();
			//}
			//else {
			//SpawnRotation = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation();
			//}
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			//const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			//FActorSpawnParameters ActorSpawnParams;
			//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			if (MagSize != 0 && reloadCurrValue == 0) {
				//World->SpawnActor<ATestProjectProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

				AActor* MyOwner = GetOwner();

				if (MyOwner) {
					CastRay(MyOwner);
				}

				// try and play the sound if specified
				if (FireSound != NULL)
				{
					UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
				}

				GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(FireCamShake);

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Shot fired!"));
				UE_LOG(LogClass, Log, TEXT("Shot fired!"));
				MagSize--;
				reloadCurrValue = reloadTime;
			}

			if (MagSize < 1) {
				Empty = true;
			}
	}

	return true;
}

void AWeaponBase::Reload() {
	MagSize = 10;
	Empty = false;
}

void AWeaponBase::CastRay(AActor* MyOwner) {
	FVector EyeLocation;
	FRotator EyeRotation;

	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 10000);

	FVector MuzzlePos = FP_MuzzleLocation->GetComponentLocation();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;

	FHitResult hit;

	if (GetWorld()->LineTraceSingleByChannel(hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams)) {
		DrawDebugLine(GetWorld(), MuzzlePos, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
		AActor* hitActor = hit.GetActor();

		ANPCBase* npc = Cast<ANPCBase>(hitActor);

		if (npc != nullptr)
		{
			if (npc->type != NULL && npc->type == 'n') {
				
				bool npcDeathState = npc->AdjustHealth(Damage);

				if (npcDeathState) {
					npc->ApplyDeathAnim();
				}
				else {
					UGameplayStatics::ApplyDamage(hitActor, Damage, MyOwner->GetInstigatorController(), MyOwner, NULL);
				}
			}
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), MuzzlePos, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
	}

	if (BeamEffect) {
		UParticleSystemComponent* BeamComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamEffect, MeshComp->GetSocketLocation("Muzzle"));

		BeamComp->SetVectorParameter("BeamEnd", (hit.Actor != NULL) ? hit.ImpactPoint : TraceEnd);
	}
}