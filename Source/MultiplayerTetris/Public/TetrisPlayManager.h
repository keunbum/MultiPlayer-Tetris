// Copyright KeunBeom Ryu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Tetrimino.h"

#include "TetrisPlayManager.generated.h"

class ATetrimino;
class ATetrisGameModeBase;
class ABoard;
class UInputComponent;

UENUM()
enum class ELockDownOption : uint8
{
	ExtendedPlacement,
	InfinitePlacement,
	Classic
};

UENUM()
enum class EPhase : uint8
{
	None,
	Generation,
	Falling,
	LockDown,
	Pattern,
	Iterate,
	Animate,
	Elimate,
	Completion
};

/**
 * @class ATetrisPlayManager
 * @brief The ATetrisPlayManager class is responsible for managing the gameplay logic of the Tetris game.
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrisPlayManager : public AActor
{
	GENERATED_BODY()
	
public:
	ATetrisPlayManager();

	virtual void Tick(const float DeltaTime) override;

	void StartGenerationPhase();
	void SetTetriminoInPlay(ATetrimino* const NewTetriminoInPlay) { TetriminoInPlay = NewTetriminoInPlay; }
	void SetNormalFallSpeed(const float NewNormalFallSpeed) { NormalFallSpeed = NewNormalFallSpeed; }

	const FVector2D& GetMovementDirection() const { return CurrentMovementDirection; }

	// Event Handlers
	void StartMovement(const FVector2D& InMovementDirection);
	void EndMovement();
	void StartSoftDrop();
	void EndSoftDrop();
	void DoHardDrop();
	void DoRotation(const ETetriminoRotationDirection RotationDirection);

protected:
	virtual void BeginPlay() override;

private:
	void Initialize();

	bool IsTetriminoManipulable() const { return Phase == EPhase::Falling; }

	void MoveTetriminoTo(const FVector2D& Direction);
	void MoveTetriminoToCurrentDirection();
	void MoveTetriminoDown();

	void RunSuperRotationSystem(const ETetriminoRotationDirection RotationDirection);

	void LockDown();

	void SetAutoRepeatMovement();
	void SetSoftDropTimer();
	void SetNormalFallTimer();
	void SetLockDownTimer();

	void ClearTimer(FTimerHandle& InOutTimerHandle);

	void SetMovementDirection(const FVector2D& NewMovementDirection) { CurrentMovementDirection = NewMovementDirection; }

	ATetrimino* SpawnNextTetrimino() const;
	void ChangeTetrimino(ATetrimino* const NewTetrimino);

	static FIntPoint GetMovementIntVector2D(const FVector2D& Direction);

private:
	static constexpr bool bIsNormalFallTimerLoop = true;
	static constexpr float NormalFallTimerInitialDelay = 0.0f;

	static constexpr bool bSoftDropTimerLoop = true;
	static constexpr float SoftDropTimerInitialDelay = 0.0f;

	static constexpr bool bIsAutoRepeatMovementLoop = true;
	static constexpr float AutoRepeatMovementInitialDelay = 0.3f;
	static constexpr float AutoRepeatMovementInterval = 0.05f; // Adjust this value as needed

	// LockDown
	static constexpr bool bIsLockDownTimerLoop = false;
	static constexpr float LockDownTimerInitialDelay = 0.5f;

private:
	float NormalFallSpeed;

	UPROPERTY(VisibleAnywhere)
	EPhase Phase;

	UPROPERTY()
	ELockDownOption LockDownOption;

	UPROPERTY()
	TObjectPtr<ATetrisGameModeBase> GameMode;

	UPROPERTY()
	TObjectPtr<ABoard> Board;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATetrimino> TetriminoClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetrimino> TetriminoInPlay;

	FTimerHandle NormalFallTimerHandle;
	FTimerHandle SoftDropTimerHandle;
	FTimerHandle AutoRepeatMovementTimerHandle;
	FTimerHandle LockDownTimerHandle;
	FTimerHandle GenerationPhaseTimerHandle;

	UPROPERTY(VisibleAnywhere)
	FVector2D CurrentMovementDirection;

	UPROPERTY(EditDefaultsOnly)
	ETetriminoShape TestSpawnType = ETetriminoShape::None;
};
