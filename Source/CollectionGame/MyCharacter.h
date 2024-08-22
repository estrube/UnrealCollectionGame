// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Item.h"
#include "MyInventory.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class COLLECTIONGAME_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void CanInteract(bool able, FString area, FString item, int amount, FVector facePoint, int i, AActor* resource);

	UPROPERTY(BlueprintReadOnly)
		FString nearByInteraction;

	UPROPERTY(BlueprintReadOnly)
		UMyInventory* inventory;

private:
	void Interact();
	void CollectFromArea();

	UPROPERTY()
		bool canInteract;
	UPROPERTY()
		bool isInteracting;
	UPROPERTY()
		FString interactionType;
	UPROPERTY()
		FString addType;
	UPROPERTY()
		int addAmount;
	UPROPERTY()
		FVector lookPoint;
	UPROPERTY()
		int typeIndex;
	UPROPERTY()
		AActor* areaObject;

	UPROPERTY()
		float counter;
	UPROPERTY()
		float delay;
	
	void CallIterate(float value);
	void CallAdd();

	UPROPERTY()
		USphereComponent* range;
	
	//movement functions
	void HoriMove(float value);
	void VertMove(float value);

	void CheckJump();

	//rotation functions
	void HoriRot(float value);
	void VertRot(float value);

	void Switch();

	//properties
	UPROPERTY()
		UCameraComponent* cam;

	UPROPERTY()
		USpringArmComponent* arm;

	UPROPERTY()
		bool jumping;

	UPROPERTY()
		bool firstPerson;

	UFUNCTION()
		void Aminate(float value);

	UPROPERTY(EditAnywhere, Category = "Animations")
		TArray<AActor*> tools;
	UPROPERTY(EditAnywhere, Category = "Animations")
		TArray<UCurveFloat*> toolAnims;

	UPROPERTY()
		FTimeline timeline;
	UPROPERTY()
		FRotator initRot;
	UPROPERTY()
		FRotator goalRot;
};
