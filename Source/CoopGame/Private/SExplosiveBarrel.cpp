// Fill out your copyright notice in the Description page of Project Settings.

#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SHealthComponent.h"
#include "Materials/Material.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	RootComponent = MeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;			// Prevent component from ticking, and only use FireImpulse()
	RadialForceComp->bIgnoreOwningActor = true;

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));

	ExplodedMaterial = CreateDefaultSubobject<UMaterial>(TEXT("ExplodedMaterial"));

	ExplosionEffects = CreateDefaultSubobject<UParticleSystem>(TEXT("ExplosionEffects"));

	Delay = 0.0f;
	ExplosionForce = 500.0f;
	RandomSpinMin = 0.0f;
	RandomSpinMax = 10.0f;
	ExplosionDamage = 100.0f;
	InnerExplosionRadius = 50.0f;
	OuterExplosionRadius = 200.0f;
	bHasExploded = false;
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	HealthComp->OnHealthChanged.AddDynamic(this, &ASExplosiveBarrel::OnHealthChanged);
}

void ASExplosiveBarrel::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bHasExploded)
	{
		bHasExploded = true;

		if (Delay > 0.0f)
		{
			FTimerHandle ExplosionTimerHandle;
			GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ASExplosiveBarrel::Explode, Delay, false);
		}
		else
		{
			Explode();
		}
	}
}

void ASExplosiveBarrel::Explode()
{
	// Play explosion effects
	if (ExplosionEffects)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffects, GetActorLocation());
	}

	// Change the barrel material
	if (ExplodedMaterial)
	{
		MeshComp->SetMaterial(0, ExplodedMaterial);
	}

	// Damage other actors
	const TArray<AActor*> IgnoreActors;
	UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), ExplosionDamage, 0.0f, GetActorLocation(), InnerExplosionRadius, OuterExplosionRadius, 1.0f, nullptr, IgnoreActors, this);

	//Push other actors
	if (RadialForceComp)
	{
		RadialForceComp->FireImpulse();
	}

	// Launch barrel upwards
	FRotator LaunchRot = FRotator::MakeFromEuler(FVector(FMath::RandRange(0.0f, 10.0f), 0.0f, FMath::RandRange(0.0f, 360.0f)));
	FVector LaunchVec = LaunchRot.RotateVector(FVector::UpVector);
	FVector LaunchSpin = FVector(FMath::FRand(), FMath::FRand(), FMath::FRand()) * FMath::RandRange(RandomSpinMin, RandomSpinMax);
	LaunchVec *= ExplosionForce;

	MeshComp->AddImpulse(LaunchVec, NAME_None, true);
	MeshComp->AddAngularImpulseInRadians(LaunchSpin, NAME_None, true);
}

