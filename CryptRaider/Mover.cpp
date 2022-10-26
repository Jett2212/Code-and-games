// Fill out your copyright notice in the Description page of Project Settings.

#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	// ...
}

// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();
	OriginalRotation = GetOwner()->GetActorRotation();
	ShouldMove.SetNum(NumOfLocks);
}

// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsMove)
	Move(DeltaTime);

	if (IsRotate) Rotate(DeltaTime);

	
}

void UMover::SetShouldMove(bool NewShouldMove, int32 KeyNum)
{
	ShouldMove[KeyNum] = NewShouldMove;
}

void UMover::Move(float DeltaTime)
{
	IsAllTrue();
	FVector TargetLocation = OriginalLocation;
	if (AllTrue) TargetLocation = OriginalLocation + MoveOffset;

	FVector CurrentLocation = GetOwner()->GetActorLocation();
	float Speed = MoveOffset.Length() / MoveTime;
	FVector  NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
	GetOwner()->SetActorLocation(NewLocation);
}

void UMover::Rotate(float DeltaTime)
{
	IsAllTrue();
	FRotator TargetRotation = OriginalRotation;
	if (AllTrue) TargetRotation = OriginalRotation + RotationOffset;
	float Speed = RotationOffset.Yaw / RotationTime;
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, Speed);
	GetOwner()->SetActorRotation(NewRotation);


}

void UMover::IsAllTrue()
{	
	AllTrue = true;
	for(int i{}; i < ShouldMove.Num(); i++ )
	{
		if (!ShouldMove[i]) AllTrue = false;
	}
}
