// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridRangeFinderComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOJOATTHEPANIC_API UGridRangeFinderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridRangeFinderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	TArray<FVector2D> getNeighbors(FVector2D hex, FVector2D MinHex, FVector2D MaxHex);
	TArray<FVector2D> getNeighbors(FVector2D CurrentHex, TArray<FVector2D> ContainTiles);
	int PathCost(const FVector2D& a, const FVector2D& b);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Action")
	TArray<FVector2D> GetRangeTilesPositions(FVector2D StartHex, int MaxDistance, FVector2D MinHex, FVector2D MaxHex, TArray<FVector2D> ExcludesHex);

	UFUNCTION(BlueprintCallable, Category = "Action")
	TArray<FVector2D> PathGrid(FVector2D StartHex, FVector2D TargetHex, TArray<FVector2D> ContainTiles);
		
};
