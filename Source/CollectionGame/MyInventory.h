// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyInventory.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLLECTIONGAME_API UMyInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeInventory();
	void Iterate(float value);
	void AddItem(FString name, int amount);

	UPROPERTY(BlueprintReadOnly)
		TArray<FString> itemNames;
	UPROPERTY(BlueprintReadOnly)
		TArray<int> itemAmounts;

private:
	UPROPERTY()
		int inventorySize;

	UPROPERTY()
		int selectedI;
		
};
