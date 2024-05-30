// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "TetrisGameModeBase.generated.h"

class ATetrisMatrix;
class ATetrimino;

UENUM()
enum class ETetrisGameState : uint8
{
	TitleScreen,
	MainMenu,
	Gameplay,
	GamePause,
};

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrisGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATetrisGameModeBase();

	int32 GetCurrentLevel() const { return CurrentLevel; }
	void LevelUp();

	/**
	* Here, speed means the seconds it takes to move one line.
	*/
	float GetFallSpeed() const { return CalculateFallSpeed(CurrentLevel); }
	float GetSoftDropSpeed() const { return GetFallSpeed() / SoftDropMultiplier; }

	ATetrimino* GetTetriminoInPlay() const { return TetriminoInPlay; }

protected:
	virtual void BeginPlay() override;

private:
	void Initialize();
	void StartGenerationPhase();
	ATetrimino* SpawnNextTetrimino() const;

	static float CalculateFallSpeed(const int32 Level);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Tetromino")
	bool bNormalFallOff = false;

private:
	static constexpr int32 PlayerIndex = 0;
	static constexpr int32 DefaultGameLevel = 1;
	static constexpr float SoftDropMultiplier = 20.0f;

private:
	int32 CurrentLevel;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ATetrisMatrix> Matrix;

	UPROPERTY()
	TSubclassOf<ATetrimino> TetriminoClass;

	UPROPERTY()
	TObjectPtr<ATetrimino> TetriminoInPlay;
};
