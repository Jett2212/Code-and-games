// Fill out your copyright notice in the Description page of Project Settings.

#include "Rocket.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ARocket::ARocket()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particles"));
	ParticleSystemComponent->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	ProjectileMovement->InitialSpeed = 2500;
	ProjectileMovement->MaxSpeed = 2500;
}

// Called when the game starts or when spawned
void ARocket::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &ARocket::OnHit);
	if (LaunchSound)
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
}

// Called every frame
void ARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARocket::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{

	AActor *MyOwner = GetOwner();

	if (MyOwner == nullptr)
	{
		Destroy();
		return;
	}
	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		const TArray<AActor *>(IgnoreActors);

		AController *MyOwnerInstigator = MyOwner->GetInstigatorController();
		UClass *DamageTypeClass = UDamageType::StaticClass();

		UGameplayStatics::ApplyRadialDamage(this, Damage, GetActorLocation(), Radius, DamageTypeClass, IgnoreActors, MyOwner, MyOwnerInstigator);
		if (HitSound)
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		if (HitParticles)
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());

		Destroy();
	}
}
