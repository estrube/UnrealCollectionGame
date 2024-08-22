// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerBox.h"
#include "InteractionArea.generated.h"

/**
 * 
 */
UCLASS()
class COLLECTIONGAME_API AInteractionArea : public ATriggerBox
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	AInteractionArea();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void EnterArea(class AActor* overlappedActor, class AActor* otherActor);

	UFUNCTION()
		void ExitArea(class AActor* overlappedActor, class AActor* otherActor);

	UPROPERTY(EditAnywhere)
		FString areaType;
	UPROPERTY(EditAnywhere)
		FString areaItem;
	UPROPERTY(EditAnywhere)
		int areaAmount;
	UPROPERTY(EditAnywhere)
		int typeIndex;

	UPROPERTY(EditAnywhere)
		AActor* resource;
	UPROPERTY()
		float delay;
	UPROPERTY()
		float counter;
};
