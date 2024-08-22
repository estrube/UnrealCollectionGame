// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInventory.h"

// Sets default values for this component's properties
UMyInventory::UMyInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	inventorySize = 3;
	selectedI = 0;
	// ...
}


// Called when the game starts
void UMyInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitializeInventory();
}


// Called every frame
void UMyInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMyInventory::InitializeInventory()
{
	for (int i = 0; i < inventorySize; i++)
	{
		itemNames.Add("Empty");
		itemAmounts.Add(0);
	}
}

void UMyInventory::Iterate(float value)
{
	selectedI += value;
	if (selectedI < 0)
	{
		selectedI = inventorySize - 1;
	}
	else if (selectedI >= inventorySize)
	{
		selectedI = 0;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, itemNames[selectedI]);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(itemAmounts[selectedI]));

}

void UMyInventory::AddItem(FString name, int amount)
{
	int firstEmpty = -1;
	bool has = false;
	for (int i = 0; i < inventorySize; i++)
	{
		if (itemNames[i] == name)
		{
			itemAmounts[i] += amount;
			has = true;
			break;
		}
		else if (itemNames[i] == "Empty" && firstEmpty == -1)
		{
			firstEmpty = i;
		}
	}
	if (!has)
	{
		if (firstEmpty != -1)
		{
			itemNames[firstEmpty] = name;
			itemAmounts[firstEmpty] = amount;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("inventory full"));
		}
	}
}
