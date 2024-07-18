// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDSingle.generated.h"

class UTextBlock;
class ATetrisPlayerStateBase;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTETRIS_API UHUDSingle : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitializeHUD(ATetrisPlayerStateBase* const NewPlayerState);

	void UpdateDisplay();

private:
	void UpdateLevelDisplay(const int32 NewLevel);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY()
	TObjectPtr<ATetrisPlayerStateBase> PlayerState;
};
