// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetrisWidgetBase.h"
#include "PopUpWidgetBase.generated.h"

class UButtonBase;

/**
 * 
 */
UCLASS(Abstract)
class TETRIS_API UPopUpWidgetBase : public UTetrisWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnCloseClicked();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButtonBase> CloseButton;
};
