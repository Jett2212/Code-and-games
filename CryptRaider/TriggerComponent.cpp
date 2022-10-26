// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
    	PrimaryComponentTick.bCanEverTick = true;

}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (Mover == nullptr)
    {
        return;
    }
    AActor* Actor = GetAcceptableActor();
    if (Actor != nullptr)
    {
        UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if (Component != nullptr)
        {
            Component->SetSimulatePhysics(false);
            Component->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform, TEXT("Socket"));
        }
        Mover->SetShouldMove(true, KeyNum);

    }
    else
    {
        Mover->SetShouldMove(false, KeyNum);
    }

}

void UTriggerComponent::SetMover(UMover* NewMover)
{
    Mover = NewMover;
}


AActor* UTriggerComponent::GetAcceptableActor() const
{   
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    for (AActor* Actor : Actors)
    {
        if (Actor->ActorHasTag(AcceptableActorTag) && !Actor->ActorHasTag("Grabbed"))
        {
            return Actor;
        }
    }

    return nullptr;
}
