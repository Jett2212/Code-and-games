// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "Sword.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0.f;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float AShooterCharacter::GetChargePercent() const
{
	return Charge / MaxCharge;
}
bool AShooterCharacter::GetCharging() const
{
	return Charging;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Charging)
	{
		if (Charge < MaxCharge)
		Charge += GetWorld()->GetDeltaSeconds();
	}
	else
	{
		if (Charge > 1.f)
		Charge -= GetWorld()->GetDeltaSeconds();
	}

	if(!GetWorldTimerManager().IsTimerActive(SwordTimer) && Sword)
	Sword->Destroy();

	

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Charge"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Charged);
	PlayerInputComponent->BindAction(TEXT("Charge"), EInputEvent::IE_Released, this, &AShooterCharacter::Uncharged);
	PlayerInputComponent->BindAction(TEXT("Swing"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Swing);

}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("Health = %f"), Health);

	if (IsDead())
	{
		ASimpleShooterGameModeBase* GameMode =  GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	}

	return DamageToApply;
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}


void AShooterCharacter::Shoot()
{
	Gun->PullTrigger();
	Charge = 1.f;
}

float AShooterCharacter::GetCharge() const
{
	return Charge;
}

void AShooterCharacter::Charged()
{
	Charging = true;
}

void AShooterCharacter::Uncharged()
{
	Charging = false;
}

void AShooterCharacter::Swing()
{	


	if(SwingAnim && SwordClass)
	{
	Sword = GetWorld()->SpawnActor<ASword>(SwordClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SwordSocket"));
	Sword->SetOwner(this);

	GetMesh()->PlayAnimation(SwingAnim, false);

	
	GetWorldTimerManager().SetTimer(SwordTimer, 2.5f, false);
	}

}



