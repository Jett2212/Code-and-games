// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ShooterCharacter.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	NewDamage = Damage;
}

void AGun::PullTrigger()
{

	FHitResult Hit;
	FVector ShotDirection;

	if (GunTrace(Hit, ShotDirection))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Hit.Location);
		if (IsExplode)
		{
			AActor *MyOwner = GetOwner();
			const TArray<AActor *>(IgnoreActors);
			UClass *DamageTypeClass = UDamageType::StaticClass();
			if(IsNeedCharge){
				AShooterCharacter* MyShooter = Cast<AShooterCharacter>(MyOwner);
				NewDamage = Damage * MyShooter->GetCharge();
				UE_LOG(LogTemp, Warning, TEXT("%f"), MyShooter->GetCharge());
			}
			UGameplayStatics::ApplyRadialDamage(this, NewDamage, Hit.Location, Radius, DamageTypeClass, IgnoreActors, MyOwner, GetOwnerController());
		}
		else
		{
			// DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);	
			UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
			UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

			AActor *HitActor = Hit.GetActor();
			if (HitActor)
			{
				FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
				HitActor->TakeDamage(Damage, DamageEvent, GetOwnerController(), this);
			}
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AGun::GunTrace(FHitResult &Hit, FVector &ShotDirection)
{
	AController *OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
		return false;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController *AGun::GetOwnerController() const
{
	APawn *OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return nullptr;
	return OwnerPawn->GetController();
}
