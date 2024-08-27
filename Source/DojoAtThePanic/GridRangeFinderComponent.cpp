// Fill out your copyright notice in the Description page of Project Settings.


#include "GridRangeFinderComponent.h"

// Sets default values for this component's properties
UGridRangeFinderComponent::UGridRangeFinderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGridRangeFinderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	UE_LOG(LogTemp, Warning, TEXT("Grid finder"));
	
}


// Called every frame
void UGridRangeFinderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FVector2D> UGridRangeFinderComponent::GetRangeTilesPositions(FVector2D StartHex, int MaxDistance, FVector2D MinHex, FVector2D MaxHex, TArray<FVector2D> ExcludesHex)
{
    TQueue<TPair<FVector2D, int>> queue;
    queue.Enqueue({ StartHex, 0 });

    TMap<FVector2D, bool> visited;
	visited.Add(StartHex, true);

    TArray<FVector2D> hexesInRange;

    while (!queue.IsEmpty()) {
		TPair<FVector2D, int> front;
		queue.Dequeue(front);

		if (StartHex != front.Key) {
			hexesInRange.Add(front.Key);
		}

        if (front.Value < MaxDistance) {
            for (auto neighbor : getNeighbors(front.Key, MinHex, MaxHex)) {
                if (!visited.Contains(neighbor) && !ExcludesHex.Contains(neighbor)) {
                    queue.Enqueue({ neighbor, front.Value + 1 });
					visited.Add(neighbor, true);
                }
            }
        }
    }

    return hexesInRange;
}

TArray<FVector2D> UGridRangeFinderComponent::PathGrid(FVector2D StartHex, FVector2D TargetHex, TArray<FVector2D> ContainTiles) {
    TMap<FVector2D, FVector2D> came_from;
    TMap<FVector2D, int> cost_so_far;
	TArray<FVector2D> hexesPath;

	hexesPath.Empty();

	if (StartHex == TargetHex) {
		return hexesPath;
	}

    TQueue<TPair<FVector2D, int>> queue;
    queue.Enqueue({ StartHex, 0 });

    came_from.Add(StartHex, StartHex);
    cost_so_far.Add(StartHex, 0);

    while (!queue.IsEmpty()) {
        TPair<FVector2D, int> front;
		queue.Dequeue(front);

        if (front.Key == TargetHex) {
            break;
        }

        for (auto neighbor : getNeighbors(front.Key, ContainTiles)) {
                int new_cost = cost_so_far[front.Key] + 1;
                if (!cost_so_far.Contains(neighbor) || new_cost < cost_so_far[neighbor]) {
                    cost_so_far.Add(neighbor, new_cost);
                    int priority = new_cost + PathCost(neighbor, TargetHex);
                    queue.Enqueue({ neighbor, priority });
                    came_from.Add(neighbor, front.Key);
                }
        }
    }
   
    FVector2D current = TargetHex;

	if (!came_from.Contains(current)) {
		return hexesPath;
	}
	
	while (!(current == StartHex)) {
        hexesPath.Add(current);
        current = came_from[current];
    }

    return hexesPath;
}

int UGridRangeFinderComponent::PathCost(const FVector2D& a, const FVector2D& b) {
    int dx = a.X - b.X;
    int dy = a.Y - b.Y;
    return (std::abs(dx) + std::abs(dy) + std::abs(dx + dy)) / 2;
}


TArray<FVector2D> UGridRangeFinderComponent::getNeighbors(FVector2D CurrentHex, FVector2D MinHex, FVector2D MaxHex)
{
	int Offset = 1;

	if ((int) CurrentHex.X % 2 == 0) {
        Offset = -1;
	}

	TArray<FVector2D> Directions = {
		FVector2D(0, -1), FVector2D(0, 1),
		FVector2D(-1, 0), FVector2D(1, 0),
		FVector2D(-1, Offset), FVector2D(1, Offset)
	};

	TArray<FVector2D> Neighbors;
	for (auto Direction : Directions) {
		FVector2D NewNeighbor = CurrentHex + Direction;

		if (NewNeighbor.X < MinHex.X || NewNeighbor.Y < MinHex.Y) {
			continue;
		}

		if (NewNeighbor.X > MaxHex.X || NewNeighbor.Y > MaxHex.Y) {
			continue;
		}

		Neighbors.Add(NewNeighbor);
	}
	return Neighbors;
}

TArray<FVector2D> UGridRangeFinderComponent::getNeighbors(FVector2D CurrentHex, TArray<FVector2D> ContainTiles)
{
	int Offset = 1;

	if ((int)CurrentHex.X % 2 == 0) {
		Offset = -1;
	}

	TArray<FVector2D> Directions = {
		FVector2D(0, -1), FVector2D(0, 1),
		FVector2D(-1, 0), FVector2D(1, 0),
		FVector2D(-1, Offset), FVector2D(1, Offset)
	};

	TArray<FVector2D> Neighbors;
	for (auto Direction : Directions) {
		FVector2D NewNeighbor = CurrentHex + Direction;

		if (ContainTiles.Num() > 0 && !ContainTiles.Contains(NewNeighbor)) {
			continue;
		}

		Neighbors.Add(NewNeighbor);
	}
	return Neighbors;
}
