// Fill out your copyright notice in the Description page of Project Settings.

#include "SGrenadeLauncher.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"




void ASGrenadeLauncher::StartFire()
{
	if (LastFireTime + TimeBetweenShots <= GetWorld()->TimeSeconds)
	{
		Fire();
	}
}

void ASGrenadeLauncher::Fire()
{
	// If we are the client, run the RPC to fire on the server.
	if (Role < ROLE_Authority)
	{
		ServerFire();
	}

	if (ProjectileClass == nullptr)
		return;

	AActor* MyOwner = GetOwner();

	if (MyOwner)
	{
		if (CurrentAmmo <= 0)
		{
			// TODO: Play empty sound
			return;
		}

		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Cast<APawn>(GetOwner());
		
		AActor* Projectile = GetWorld()->SpawnActor(ProjectileClass, &MuzzleLocation, &EyeRotation, SpawnParams);

		// We don't need this if the projectile has a projectile movement component.
// 		if (Projectile)
// 		{
// 			UStaticMeshComponent* ProjMesh = Cast<UStaticMeshComponent>(Projectile->GetComponentByClass(UStaticMeshComponent::StaticClass()));
// 
// 			if (ProjMesh)
// 			{
// 				FVector ProjVelocity = EyeRotation.Vector() * LaunchSpeed;
// 				ProjMesh->AddImpulse(ProjVelocity);
// 			}
// 		}

		if (MuzzleEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
		}

		SetCurrentAmmo(CurrentAmmo - 1);

		LastFireTime = GetWorld()->TimeSeconds;
	}
}
