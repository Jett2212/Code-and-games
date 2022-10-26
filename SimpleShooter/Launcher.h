// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "Launcher.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ALauncher : public AGun
{
	GENERATED_BODY()

public:
	ALauncher();

	virtual void PullTrigger() override;

private: 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;


	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ARocket> RocketClass;

};
