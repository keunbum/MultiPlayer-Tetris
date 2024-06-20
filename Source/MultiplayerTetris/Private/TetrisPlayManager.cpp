// Copyright KeunBeom Ryu. All Rights Reserved.

#include "TetrisPlayManager.h"

#include "Board.h"
#include "TetrisGameModeBase.h"
#include "Tetrimino.h"

ATetrisPlayManager::ATetrisPlayManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Phase = EPhase::None;

	LockDownOption = ELockDownOption::ExtendedPlacement;

	NormalFallSpeed = -1.0f;

	TetriminoClass = ATetrimino::StaticClass();

	TetriminoInPlay = nullptr;
}

void ATetrisPlayManager::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATetrisPlayManager::StartGenerationPhase()
{
	UE_LOG(LogTemp, Display, TEXT("Start Generation Phase."));

	Phase = EPhase::Generation;
	ATetrimino* const NewTetrimino = SpawnNextTetrimino();
	check(NewTetrimino != nullptr);
	ChangeTetrimino(NewTetrimino);

	StartFallingPhase();
}

void ATetrisPlayManager::StartFallingPhase()
{
	UE_LOG(LogTemp, Display, TEXT("Start Falling Phase."));

	Phase = EPhase::Falling;
	SetNormalFallTimer();
}

void ATetrisPlayManager::StartMovement(const FVector2D& InMovementDirection)
{
	if (!IsTetriminoManipulable())
	{
		return;
	}

	check(TetriminoInPlay != nullptr);

	SetMovementDirection(InMovementDirection);
	MoveTetriminoToCurrentDirection();
	SetAutoRepeatMovement();
}

void ATetrisPlayManager::EndMovement()
{
	if (TetriminoInPlay)
	{
		ClearTimer(AutoRepeatMovementTimerHandle);
		SetMovementDirection(FVector2D::ZeroVector);
	}
}

void ATetrisPlayManager::StartSoftDrop()
{
	if (!IsTetriminoManipulable())
	{
		return;
	}

	check(TetriminoInPlay != nullptr);

	// NormalFall 일시 중지
	ClearTimer(NormalFallTimerHandle);

	SetSoftDropTimer();
}

void ATetrisPlayManager::EndSoftDrop()
{
	if (TetriminoInPlay)
	{
		ClearTimer(SoftDropTimerHandle);

		// NormalFall 재개
		SetNormalFallTimer();
	}
}

void ATetrisPlayManager::DoHardDrop()
{
	// TODO: 하드 드롭 로직 추가
}

void ATetrisPlayManager::DoRotation(const ETetriminoRotationDirection RotationDirection)
{
	if (!IsTetriminoManipulable())
	{
		return;
	}

	RunSuperRotationSystem(RotationDirection);
}

void ATetrisPlayManager::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void ATetrisPlayManager::Initialize()
{
	UWorld* const World = GetWorld();
	check(World != nullptr);
	GameMode = World->GetAuthGameMode<ATetrisGameModeBase>();

	SetNormalFallSpeed(GameMode->GetNormalFallSpeed());
	SetMovementDirection(FVector2D::ZeroVector);

	Board = World->SpawnActor<ABoard>();
	check(Board != nullptr);

	UMino::ClearMaterialCache();
}

void ATetrisPlayManager::MoveTetriminoTo(const FVector2D& Direction)
{
	if (!TetriminoInPlay)
	{
		UE_LOG(LogTemp, Display, TEXT("TetriminoInPlay is nullptr."));
		return;
	}
	
	const FIntPoint MovementIntVector2D = GetMovementIntVector2D(Direction);
	const bool bIsSoftDropOrNormalFall = (Direction == ATetrimino::MoveDirectionDown);
	const bool bIsMovementPossible = Board->IsMovementPossible(TetriminoInPlay, MovementIntVector2D);
	if (bIsMovementPossible)
	{
		TetriminoInPlay->MoveBy(MovementIntVector2D);

		const bool bIsOnSurface = !Board->IsMovementPossible(TetriminoInPlay, MovementIntVector2D);
		const bool bIsLockPhaseReached = bIsSoftDropOrNormalFall && bIsOnSurface;
		if (bIsLockPhaseReached)
		{
			UE_LOG(LogTemp, Display, TEXT("LockDown Timer is set."));
			SetLockDownTimer();
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Movement is impossible."));

		if (bIsSoftDropOrNormalFall)
		{
			ForcedLockDown();
		}
	}
}

void ATetrisPlayManager::MoveTetriminoToCurrentDirection()
{
	MoveTetriminoTo(CurrentMovementDirection);
}

void ATetrisPlayManager::MoveTetriminoDown()
{
	MoveTetriminoTo(ATetrimino::MoveDirectionDown);
}

void ATetrisPlayManager::RunSuperRotationSystem(const ETetriminoRotationDirection RotationDirection)
{
	if (!TetriminoInPlay)
	{
		return;
	}

	const TArray<FIntPoint>& RotationPointOffsets = TetriminoInPlay->GetSRSRotationPointOffsets(RotationDirection);
	for (int32 PointIndex = 0; PointIndex < RotationPointOffsets.Num(); ++PointIndex)
	{
		const FIntPoint& RotationPointOffset = RotationPointOffsets[PointIndex];
		if (Board->IsRotationPossible(TetriminoInPlay, RotationDirection, RotationPointOffset))
		{
			TetriminoInPlay->RotateTo(RotationDirection);
			TetriminoInPlay->MoveBy(RotationPointOffset);
			//UE_LOG(LogTemp, Display, TEXT("%Rotation with Point%d."), PointIndex + 1);
			return;
		}
	}
	//UE_LOG(LogTemp, Display, TEXT("All of Rotation Points failed."));
}

void ATetrisPlayManager::LockDown()
{
	UE_LOG(LogTemp, Display, TEXT("Lock Down."));

	Phase = EPhase::LockDown;

	check(TetriminoInPlay != nullptr);

	PlayLockDownEffect(TetriminoInPlay->GetMinoArray());

	// Add minos from TetriminoInPlay to the board
	Board->AddMinos(TetriminoInPlay);

	// Remove TetriminoInPlay
	TetriminoInPlay->Destroy();
	TetriminoInPlay = nullptr;

	// Switch to Generation Phase.
	static constexpr bool bIsGenerationPhaseTimerLoop = false;
	GetWorldTimerManager().SetTimer(GenerationPhaseTimerHandle, this, &ATetrisPlayManager::StartGenerationPhase, GenerationPhaseInitialDelay, bIsGenerationPhaseTimerLoop);
}

void ATetrisPlayManager::ForcedLockDown()
{
	ClearTimer(LockDownTimerHandle);
	LockDown();
}

void ATetrisPlayManager::SetAutoRepeatMovement()
{
	GetWorldTimerManager().SetTimer(AutoRepeatMovementTimerHandle, this, &ATetrisPlayManager::MoveTetriminoToCurrentDirection, AutoRepeatMovementInterval, bIsAutoRepeatMovementLoop, AutoRepeatMovementInitialDelay);
}

void ATetrisPlayManager::SetSoftDropTimer()
{
	GetWorldTimerManager().SetTimer(SoftDropTimerHandle, this, &ATetrisPlayManager::MoveTetriminoDown, GameMode->GetSoftDropSpeed(), bSoftDropTimerLoop, SoftDropTimerInitialDelay);
}

void ATetrisPlayManager::SetNormalFallTimer()
{
	if (GameMode && !GameMode->bNormalFallOff)
	{
		UE_LOG(LogTemp, Display, TEXT("Normal Fall Timer is set."));
		GetWorldTimerManager().SetTimer(NormalFallTimerHandle, this, &ATetrisPlayManager::MoveTetriminoDown, NormalFallSpeed, bIsNormalFallTimerLoop, NormalFallTimerInitialDelay);
	}
}

void ATetrisPlayManager::SetLockDownTimer()
{
	GetWorldTimerManager().SetTimer(LockDownTimerHandle, this, &ATetrisPlayManager::LockDown, LockDownTimerInitialDelay, bIsLockDownTimerLoop);
}

void ATetrisPlayManager::ClearTimer(FTimerHandle& InOutTimerHandle)
{
	GetWorldTimerManager().ClearTimer(InOutTimerHandle);
}

ATetrimino* ATetrisPlayManager::SpawnNextTetrimino() const
{
	if (ATetrimino* const NewTetrimino = GetWorld()->SpawnActor<ATetrimino>(TetriminoClass))
	{
#define TETRIMINO_SPAWN_RANDOM 1

#if TETRIMINO_SPAWN_RANDOM == 1
		const ETetriminoShape NewTetriminoType = ATetrimino::GetTetriminoShapeRandom();
		NewTetrimino->Initialize(NewTetriminoType);
#else
		NewTetrimino->Initialize(TestSpawnType);
#endif
		return NewTetrimino;
	}
	return nullptr;
}

void ATetrisPlayManager::ChangeTetrimino(ATetrimino* const NewTetrimino)
{
	check(NewTetrimino != nullptr);
	NewTetrimino->AttachToBoard(Board);
	SetTetriminoInPlay(NewTetrimino);
}

void ATetrisPlayManager::PlayLockDownEffect(const TArray<UMino*>& GetMinoArray)
{
}

FIntPoint ATetrisPlayManager::GetMovementIntVector2D(const FVector2D& Direction)
{
	static constexpr float OneSpace = 1.0f;
	const FVector2D MovementVector2D = OneSpace * Direction;
	return FIntPoint(static_cast<int32>(MovementVector2D.X), static_cast<int32>(MovementVector2D.Y));
}
