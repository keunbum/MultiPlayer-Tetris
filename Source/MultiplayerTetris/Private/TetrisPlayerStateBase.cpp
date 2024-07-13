// Copyright Ryu KeunBeom. All Rights Reserved.

#include "TetrisPlayerStateBase.h"

#include "TetrisGameModeBase.h"

ATetrisPlayerStateBase::ATetrisPlayerStateBase()
	: GameLevel(ATetrisGameModeBase::DefaultGameLevel)
	, LineClearCount(ATetrisGameModeBase::DefaultLineCount)
{
	SetScore(ATetrisGameModeBase::DefaultScore);
}

void ATetrisPlayerStateBase::UpdateState(const FTetrisGamePlayInfo& UpdateInfo)
{
	AddLineClearCount(UpdateInfo.HitList.Num());

	UE_LOG(LogTemp, Warning, TEXT("Total Cleared LineClearCount: %d"), GetLineClearCount());
}
