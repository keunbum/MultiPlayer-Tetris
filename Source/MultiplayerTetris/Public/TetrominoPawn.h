// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/EngineTypes.h"

#include "TetrominoPawn.generated.h"

class ATetrimino;
class ATetrisGameModeBase;
class UInputComponent;

UCLASS()
class MULTIPLAYERTETRIS_API ATetrominoPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATetrominoPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(const float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* const PlayerInputComponent) override;

public:
	void SetTetrominoInPlay(ATetrimino* const NewTetrominoInPlay) { TetrominoInPlay = NewTetrominoInPlay; }
	void SetNormalFallSpeed(const float NewNormalFallSpeed) { NormalFallSpeed = NewNormalFallSpeed; }

	FVector2D GetMovementDirection() const { return MovementDirection; }

	// Event Handlers
	void StartMovement(const FVector2D& InMovementDirection);
	void EndMovement();
	void StartSoftDrop();
	void EndSoftDrop();
	void StartHardDrop();
	/**
	* @param RotationDirection - +1: 시계 방향, -1: 반시계 방향.
	*/
	void StartRotate(const int32 RotationDirection);

private:
	void Initialize();
	void ClearTimer(FTimerHandle& InOutTimerHandle);

	void MoveTo(const FVector2D& Direction);
	void MoveToCurrentDirection();
	void MoveDown();

	void SetAutoRepeatMovement();
	void SetNormalFallTimer();

	void SetMovementDirection(const FVector2D& NewMovementDirection) { MovementDirection = NewMovementDirection; }

private:
	static constexpr bool bIsNormalFallTimerLoop = true;
	static constexpr float NormalFallTimerInitialDelay = 0.0f;

	static constexpr bool bSoftDropTimerLoop = true;
	static constexpr float SoftDropTimerInitialDelay = 0.0f;

	static constexpr bool bIsAutoRepeatMovementLoop = true;
	static constexpr float AutoRepeatMovementInitialDelay = 0.3f;
	static constexpr float AutoRepeatMovementInterval = 0.05f; // Adjust this value as needed

private:
	float NormalFallSpeed;

	UPROPERTY()
	TObjectPtr<ATetrisGameModeBase> GameMode;

	// 조작 중인 테트로미노
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetrimino> TetrominoInPlay;

	// 타이머 핸들러
	FTimerHandle NormalFallTimerHandle;
	FTimerHandle SoftDropTimerHandle;
	FTimerHandle AutoRepeatMovementTimerHandle;

	UPROPERTY(VisibleAnywhere)
	FVector2D MovementDirection;
};
