// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class USHealthComponent;
class URadialForceComponent;
class UMaterial;
class UParticleSystem;

UCLASS()
class COOPGAME_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URadialForceComponent* RadialForceComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosive Barrel")
	UMaterial* ExplodedMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosive Barrel")
	UParticleSystem* ExplosionEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosive Barrel")
	float Delay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosive Barrel")
	float ExplosionForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosive Barrel")
	float RandomSpinMin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosive Barrel")
	float RandomSpinMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosive Barrel")
	float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosive Barrel")
	float InnerExplosionRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosive Barrel")
	float OuterExplosionRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool bHasExploded;

	UFUNCTION()
	void OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void Explode();

public:

	
	
};
