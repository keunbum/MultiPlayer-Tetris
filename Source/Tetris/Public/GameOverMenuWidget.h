// Copyright Ryu KeunBeom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidgetBase.h"
#include "GameoverMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API UGameoverMenuWidget : public UMenuWidgetBase
{
	GENERATED_BODY()
	
protected:
	/** UUserWidget Interface */
	virtual void NativeOnInitialized() override;
	/** ~UUserWidget Interface */

private:
	UFUNCTION()
	void OnRestartClicked();

	UFUNCTION()
	void OnHomeClicked();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> RestartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuButton> HomeButton;
};
