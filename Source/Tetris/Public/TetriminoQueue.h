// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Mino.h"

#include "TetriminoQueue.generated.h"

class ATetrimino;
class USceneComponent;

/**
 * ATetriminoQueue is an Actor responsible for managing a queue of Tetriminos in the game.
 * It allows Tetriminos to be added to the queue and provides functionality to retrieve
 * and remove the next Tetrimino from the queue when needed. This system is essential
 * for controlling the flow of Tetriminos that the player will place on the game board.
 */
UCLASS()
class TETRIS_API ATetriminoQueue : public AActor
{
	GENERATED_BODY()
	
public:	
	ATetriminoQueue();

	/**
	 * Initializes the Tetrimino queue with the specified maximum queue size.
	 * @param MaxQueueSize The maximum number of Tetriminos that can be stored in the queue.
	 * @param ParentComponent The component that will be the root of the Tetrimino queue.
	 */
	void Initialize(const int32 QueueSize, USceneComponent* const NewParentComponent);

	/**
	 * Adds a Tetrimino to the queue.
	 * @param Tetrimino The Tetrimino to add to the queue.
	 */
	void Enqueue(ATetrimino* const Tetrimino);

	/**
	 * Removes and returns the next Tetrimino from the queue.
	 * @return The next Tetrimino in the queue, or nullptr if the queue is empty.
	 */
	ATetrimino* Dequeue();

	void ReArrangeTetriminoLocations();

	int32 Num() const { return TetriminoArray.Num(); }
	bool IsEmpty() const { return TetriminoArray.IsEmpty(); }
	ATetrimino* Last() const { return IsEmpty() ? nullptr : TetriminoArray.GetData()[Num() - 1]; }
	ATetrimino* First() const { return IsEmpty() ? nullptr : TetriminoArray.GetData()[0]; }

private:
	static FVector GetTetriminoLocalLocationByIndex(const int32 Index);

private:
	static constexpr float TetriminoYOffset = -(UMino::UnitLength * 4.0f);

private:
	UPROPERTY()
	TArray<TObjectPtr<ATetrimino>> TetriminoArray;
};
