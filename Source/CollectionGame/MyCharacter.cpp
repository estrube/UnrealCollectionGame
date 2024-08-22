// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bUseControllerRotationYaw = false;

	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	//childs parent to root, gives it a radius, and then sets the wanted rotation
	arm->AttachTo(RootComponent);
	arm->TargetArmLength = 300.f;
	arm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	//gives camera slower movement than player
	arm->bEnableCameraLag = true;
	arm->CameraLagSpeed = 2;
	arm->CameraLagMaxDistance = 1.5f;

	//gives camera slower rotation than player
	arm->bEnableCameraRotationLag = true;
	arm->CameraRotationLagSpeed = 4;
	arm->CameraLagMaxTimeStep = 1;

	//positions camera to be at the end of the arm (radius)
	cam->AttachTo(arm, USpringArmComponent::SocketName);

	//defaulting properties
	jumping = false;

	firstPerson = false;

	range = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionRange"));
	range->AttachTo(RootComponent);
	range->SetSphereRadius(100.0f);

	canInteract = false;
	isInteracting = false;
	interactionType = "null";
	addType = "null";
	addAmount = 0;
	counter = 0;
	delay = 0;

	nearByInteraction = "null";
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	inventory = this->FindComponentByClass<UMyInventory>();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	timeline.TickTimeline(DeltaTime);

	if (jumping)
	{
		Jump();
	}

	if (isInteracting)
	{
		counter += DeltaTime;
		if (counter >= delay)
		{
			CollectFromArea();
		}
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Interacting", IE_Pressed, this, &AMyCharacter::Interact);

	InputComponent->BindAction("Pickup", IE_Pressed, this, &AMyCharacter::CallAdd);
	InputComponent->BindAxis("Iterate", this, &AMyCharacter::CallIterate);

	//axes
	//movement
	InputComponent->BindAxis("Horizontal", this, &AMyCharacter::HoriMove);
	InputComponent->BindAxis("Vertical", this, &AMyCharacter::VertMove);

	//rotation
	InputComponent->BindAxis("HorizontalRot", this, &AMyCharacter::HoriRot);
	InputComponent->BindAxis("VerticalRot", this, &AMyCharacter::VertRot);

	//actions
	//movement
	InputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::CheckJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter::CheckJump);

	InputComponent->BindAction("ChangeCamera", IE_Pressed, this, &AMyCharacter::Switch);
}

void AMyCharacter::Aminate(float value)
{
	FRotator rot = FMath::Lerp(initRot, goalRot, value);

	tools[typeIndex]->SetActorRotation(rot);
}

void AMyCharacter::CanInteract(bool able, FString area, FString item, int amount, FVector facePoint, int i, AActor* resource)
{
	canInteract = able;
	interactionType = area;
	addType = item;
	addAmount = amount;
	lookPoint = facePoint;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("can interact"));
	nearByInteraction = area;

	if (resource)
	{
		areaObject = resource;
	}

	typeIndex = i;
	if (&toolAnims[typeIndex])
	{
		FOnTimelineFloat animation;
		animation.BindUFunction(this, FName("Aminate"));
		timeline.AddInterpFloat(toolAnims[typeIndex], animation);
		timeline.SetLooping(false);
	}
}

void AMyCharacter::Interact()
{
	if (interactionType != "null" && canInteract && !isInteracting)
	{
		isInteracting = true;
		counter = 0;
		delay = 3;
		nearByInteraction = "null";
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), lookPoint);
		SetActorRotation(FRotator(0, rot.Yaw, 0));

		initRot = tools[typeIndex]->GetActorRotation();
		goalRot = initRot;
		goalRot.Roll += 60;

		if (!timeline.IsPlaying())
		{
			tools[typeIndex]->SetActorHiddenInGame(false);
			timeline.PlayFromStart();
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("is interacting"));
	}
	else if (interactionType != "null" && canInteract && isInteracting)
	{
		isInteracting = false;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("exited interact"));
	}
}

void AMyCharacter::CollectFromArea()
{
	if (inventory)
	{
		if (interactionType != "null")
		{
			inventory->AddItem(addType, addAmount);
			isInteracting = false;
			tools[typeIndex]->SetActorHiddenInGame(true);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("finished interaction"));
			canInteract = false;

			if (areaObject)
			{
				areaObject->SetActorHiddenInGame(true);
				areaObject->SetActorEnableCollision(false);
			}
		}
	}
}

void AMyCharacter::CallIterate(float value)
{
	if (value && inventory)
	{
		inventory->Iterate(value);
	}
}

void AMyCharacter::CallAdd()
{
	if (inventory)
	{
		TArray<AActor*> collectionItems;
		range->GetOverlappingActors(collectionItems);

		for (int i = 0; i < collectionItems.Num(); i++)
		{
			AItem* const tempItem = Cast<AItem>(collectionItems[i]);
			if (tempItem && !tempItem->IsPendingKill() && tempItem->GetActive())
			{
				inventory->AddItem(tempItem->name, tempItem->amount);
				tempItem->Touched();
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, tempItem->name);
			}
		}
	}
}

//movement
void AMyCharacter::HoriMove(float value)
{
	if (value && !isInteracting)
	{
		AddMovementInput(GetActorRightVector(), value);
	}
}

void AMyCharacter::VertMove(float value)
{
	if (value && !isInteracting)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void AMyCharacter::CheckJump()
{
	jumping = !jumping;
}

//rotation
void AMyCharacter::HoriRot(float value)
{
	if (value && !isInteracting)
	{
		AddActorLocalRotation(FRotator(0, value, 0));
	}
}

void AMyCharacter::VertRot(float value)
{
	float temp = 0;
	if (firstPerson && !isInteracting)
	{
		temp = cam->GetRelativeRotation().Pitch + value;

		if (temp < 65 && temp > -65)
		{
			cam->AddLocalRotation(FRotator(value, 0, 0));
		}
	}
	else
	{
		//adjusts zoom (radius size)
		temp = arm->GetRelativeRotation().Pitch + value;
		//checks if player is inside zoom range
		if (temp < 25 && temp > -65)
		{
			arm->AddLocalRotation(FRotator(value, 0, 0));
		}
	}

}

void AMyCharacter::Switch()
{
	if (firstPerson)
	{
		arm->TargetArmLength = 300.f;
		arm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
		cam->SetRelativeRotation(FRotator(0, 0, 0));
		cam->AttachTo(arm, USpringArmComponent::SocketName);
		firstPerson = false;
	}
	else
	{
		cam->AttachTo(RootComponent);
		cam->SetRelativeLocation(FVector(-1, 0, 17));
		firstPerson = true;
	}
}

