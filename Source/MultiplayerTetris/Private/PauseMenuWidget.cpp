// Copyright Ryu KeunBeom. All Rights Reserved.

#include "PauseMenuWidget.h"

#include "Kismet/GameplayStatics.h"

#include "MenuButton.h"
#include "TetrisPlayerControllerSingle.h"
#include "MainMenuGameMode.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MenuButtons.IsEmpty())
	{
		SetMenuButtons({ ResumeButton, RestartButton, ExitButton });
	}

	SetDefaultMenuButtonFocus();

	if (!ResumeButton->OnClicked.IsBound())
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);
	}
	if (!RestartButton->OnClicked.IsBound())
	{
		RestartButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnRestartClicked);
	}
	if (!ExitButton->OnClicked.IsBound())
	{
		ExitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnExitClicked);
	}

	if (!TetrisPlayerController)
	{
		TetrisPlayerController = Cast<ATetrisPlayerControllerSingle>(GetOwningPlayer());
	}
}

FReply UPauseMenuWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();

	if (static const TArray<FKey> ResumeKeys = { EKeys::Q, EKeys::BackSpace };
		ResumeKeys.Contains(Key))
	{
		OnResumeClicked();
		return FReply::Handled();
	}

	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

void UPauseMenuWidget::OnResumeClicked()
{
	if (TetrisPlayerController)
	{
		TetrisPlayerController->SetInputModeGameOnlyAndUnPause();
	}
}

void UPauseMenuWidget::OnRestartClicked()
{
	// Restart the level
	UGameplayStatics::OpenLevel(this, GetWorld()->GetFName());
}

void UPauseMenuWidget::OnExitClicked()
{
	// Exit to Main Menu Level
	UGameplayStatics::OpenLevel(this, AMainMenuGameMode::MainMenuLevelName);
}
