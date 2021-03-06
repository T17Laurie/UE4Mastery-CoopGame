// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAmmoChangedDelegate, ASWeapon*, Weapon);

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ASWeapon();

	virtual void StartFire();

	virtual void StopFire();

	virtual void Reload();

	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo()		{ return CurrentAmmo; }
	
	UFUNCTION(BlueprintPure)
	int32 GetAmmoCapacity()		{ return AmmoCapacity; }

	UPROPERTY(BlueprintAssignable)
	FAmmoChangedDelegate OnAmmoChanged;

protected:

	virtual void BeginPlay() override;

	virtual void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	FTimerHandle TimerHandle_TimeBetweenShots;

	float LastFireTime;

	// RPM - Bullets per minute fired by weapon
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;

	// Derived from RateOfFire
	float TimeBetweenShots;

	// Ammo
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 AmmoCapacity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 CurrentAmmo;

	void SetCurrentAmmo(int32 NewAmmoAmt);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	void PlayFireEffects(FVector TraceEnd);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem*  MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem*  DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem*  FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem*  TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShake> FireCamShake;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage;
};
