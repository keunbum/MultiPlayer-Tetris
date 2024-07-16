// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "TetrisGameModeBase.h"

#include "TetrisPlayerStateBase.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrisPlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
	ATetrisPlayerStateBase();

	void AddScore(const int32 AddedScore) { SetScore(GetScore() + AddedScore); }

	int32 GetGameLevel() const { return GameLevel; }
	void SetGameLevel(const int32 NewGameLevel) { GameLevel = NewGameLevel; }
	void AddGameLevel(const int32 AddedGameLevel) { SetGameLevel(GetGameLevel() + AddedGameLevel); }
	void LevelUp(const int32 LevelUpLineCountGoal);

	int32 GetLineClearCount() const { return LineClearCount; }
	void SetLineClearCount(const int32 NewLineClearCount) { LineClearCount = NewLineClearCount; }
	void AddLineClearCount(const int32 AddedLineClearCount) { SetLineClearCount(GetLineClearCount() + AddedLineClearCount); }

	int32 GetTotalLineClearCount() const { return TotalLineClearCount; }
	void SetTotalLineClearCount(const int32 NewTotalLineClearCount) { TotalLineClearCount = NewTotalLineClearCount; }
	void AddTotalLineClearCount(const int32 AddedTotalLineClearCount) { SetTotalLineClearCount(GetTotalLineClearCount() + AddedTotalLineClearCount); }

	void UpdateState(const FTetrisGamePlayInfo& UpdateInfo);

private:
	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 GameLevel;

	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 LineClearCount;

	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int32 TotalLineClearCount;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HUDSingleClass = nullptr;
};
