// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetShouldMove(bool ShouldMove, int32 KeyNum);

private:

// Move Properties
	UPROPERTY(EditAnywhere)
	bool IsMove = true;

	UPROPERTY(EditAnywhere)
	FVector MoveOffset;

	UPROPERTY(EditAnywhere)
	float MoveTime {4};
	
	FVector OriginalLocation;


// Rotation Properties
	UPROPERTY(EditAnywhere)
	bool IsRotate;

	UPROPERTY(EditAnywhere)
	FRotator RotationOffset;

	UPROPERTY(EditAnywhere)
	float RotationTime {4};

	FRotator OriginalRotation;

// Shared Properties
	TArray<bool> ShouldMove;

	UPROPERTY(EditAnywhere)
	int32 NumOfLocks {1};

	bool AllTrue = true;

	void IsAllTrue();

// Function Declaration
	void Rotate(float DeltaTime);

	void Move(float DeltaTime);
};
