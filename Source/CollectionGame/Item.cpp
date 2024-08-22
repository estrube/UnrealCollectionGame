// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	name = "test";
	amount = 1;

	isVisible = true;

	itemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("itemMesh"));
	RootComponent = itemMesh;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AItem::GetActive()
{
	return isVisible;
}

void AItem::Touched()
{
	isVisible = false;
	this->SetActorHiddenInGame(true);
	this->SetActorEnableCollision(false);
}

