// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionArea.h"

AInteractionArea::AInteractionArea()
{
	PrimaryActorTick.bCanEverTick = true;

	OnActorBeginOverlap.AddDynamic(this, &AInteractionArea::EnterArea);
	OnActorEndOverlap.AddDynamic(this, &AInteractionArea::ExitArea);

	delay = 3;
	counter = 0;
}

void AInteractionArea::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractionArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (resource)
	{
		if (!resource->GetActorEnableCollision())
		{
			counter += DeltaTime;
			if (counter > delay)
			{
				resource->SetActorHiddenInGame(false);
				resource->SetActorEnableCollision(true);
				counter = 0;
			}
		}
	}
}

void AInteractionArea::EnterArea(class AActor* overlappedActor, class AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		AMyCharacter* character = Cast<AMyCharacter>(otherActor);
		if (character && (!resource || resource->GetActorEnableCollision()))
		{
			character->CanInteract(true, areaType, areaItem, areaAmount, GetActorLocation(), typeIndex, resource);
		}
	}
}

void AInteractionArea::ExitArea(class AActor* overlappedActor, class AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		AMyCharacter* character = Cast<AMyCharacter>(otherActor);
		if (character)
		{
			character->CanInteract(false, "null", "null", 0, FVector(0,0,0), 0, NULL);
		}
	}
}