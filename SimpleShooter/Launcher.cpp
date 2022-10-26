// Fill out your copyright notice in the Description page of Project Settings.


#include "Launcher.h"
#include "Kismet/GameplayStatics.h"
#include "Rocket.h"


ALauncher::ALauncher()
{
    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
}

void ALauncher::PullTrigger()
{
    FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	ARocket* Rocket = GetWorld()->SpawnActor<ARocket>(RocketClass, Location, Rotation);
	Rocket->SetOwner(this);
}

