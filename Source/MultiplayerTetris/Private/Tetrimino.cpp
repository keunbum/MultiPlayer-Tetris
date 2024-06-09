// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#include "Tetrimino.h"

#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

#include "Mino.h"
#include "Board.h"

const FVector2D ATetrimino::MoveDirectionLeft = FVector2D(0, -1);
const FVector2D ATetrimino::MoveDirectionRight = -MoveDirectionLeft;
const FVector2D ATetrimino::MoveDirectionDown = MoveDirectionLeft.GetRotated(90.f);

const TMap<ETetriminoShape, FTetriminoInfo> ATetrimino::TetriminoInfos =
{
	{
		ETetriminoShape::O,
		{
			{
				{ ETetriminoFacing::North, { {1, 1}, {1, 2}, {2, 1}, {2, 2} } },
				{ ETetriminoFacing::East, { {1, 1}, {1, 2}, {2, 1}, {2, 2} } },
				{ ETetriminoFacing::South, { {1, 1}, {1, 2}, {2, 1}, {2, 2} } },
				{ ETetriminoFacing::West, { {1, 1}, {1, 2}, {2, 1}, {2, 2} } }
			},                                            // TMap<ETetriminoFacing, TArray<FVector2D>>
			TEXT("/Game/Material/M_MinoMaterial_Yellow"), // FString
			FIntVector2(ABoard::TetriminoDefaultSpawnLocationX - 2, ABoard::TetriminoDefaultSpawnLocationY),                              // FIntPoint
		}
	},
	{
		ETetriminoShape::I,
		{
			{
				{ ETetriminoFacing::North, { {1, 0}, {1, 1}, {1, 2}, {1, 3} } },
				{ ETetriminoFacing::East, { {0, 2}, {1, 2}, {2, 2}, {3, 2} } },
				{ ETetriminoFacing::South, { {2, 0}, {2, 1}, {2, 2}, {2, 3} } },
				{ ETetriminoFacing::West, { {0, 1}, {1, 1}, {2, 1}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Cyan"),
			FIntVector2(ABoard::TetriminoDefaultSpawnLocationX - 1, ABoard::TetriminoDefaultSpawnLocationY),                              // FIntPoint
		}
	},
	{
		ETetriminoShape::T,
		{
			{
				{ ETetriminoFacing::North, { {1, 1}, {2, 0}, {2, 1}, {2, 2} } },
				{ ETetriminoFacing::East, { {1, 1}, {2, 1}, {2, 2}, {3, 1} } },
				{ ETetriminoFacing::South, { {2, 0}, {2, 1}, {2, 2}, {3, 1} } },
				{ ETetriminoFacing::West, { {1, 1}, {2, 0}, {2, 1}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Purple"),
			FIntVector2(ABoard::TetriminoDefaultSpawnLocationX - 2, ABoard::TetriminoDefaultSpawnLocationY)                               // FIntPoint
		}
	},
	{
		ETetriminoShape::L,
		{
			{
				{ ETetriminoFacing::North, { {1, 2}, {2, 0}, {2, 1}, {2, 2} } },
				{ ETetriminoFacing::East, { {1, 1}, {2, 1}, {3, 1}, {3, 2} } },
				{ ETetriminoFacing::South, { {2, 0}, {2, 1}, {2, 2}, {3, 0} } },
				{ ETetriminoFacing::West, { {1, 0}, {1, 1}, {2, 1}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Orange"),
			FIntVector2(ABoard::TetriminoDefaultSpawnLocationX - 2, ABoard::TetriminoDefaultSpawnLocationY)                               // FIntPoint
		}
	},
	{
		ETetriminoShape::J,
		{
			{
				{ ETetriminoFacing::North, { {1, 0}, {2, 0}, {2, 1}, {2, 2} } },
				{ ETetriminoFacing::East, { {1, 1}, {1, 2}, {2, 1}, {3, 1} } },
				{ ETetriminoFacing::South, { {2, 0}, {2, 1}, {2, 2}, {3, 2} } },
				{ ETetriminoFacing::West, { {1, 1}, {2, 1}, {3, 0}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Blue"),
			FIntVector2(ABoard::TetriminoDefaultSpawnLocationX - 2, ABoard::TetriminoDefaultSpawnLocationY)                               // FIntPoint
		}
	},
	{
		ETetriminoShape::S,
		{
			{
				{ ETetriminoFacing::North, { {1, 1}, {1, 2}, {2, 0}, {2, 1} } },
				{ ETetriminoFacing::East, { {1, 1}, {2, 1}, {2, 2}, {3, 2} } },
				{ ETetriminoFacing::South, { {2, 1}, {2, 2}, {3, 0}, {3, 1} } },
				{ ETetriminoFacing::West, { {1, 0}, {2, 0}, {2, 1}, {3, 1} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Green"),
			FIntVector2(ABoard::TetriminoDefaultSpawnLocationX - 2, ABoard::TetriminoDefaultSpawnLocationY)                               // FIntPoint
		}
	},
	{
		ETetriminoShape::Z,
		{
			{
				{ ETetriminoFacing::North, { {1, 0}, {1, 1}, {2, 1}, {2, 2} } },
				{ ETetriminoFacing::East, { {1, 2}, {2, 1}, {2, 2}, {3, 1} } },
				{ ETetriminoFacing::South, { {2, 0}, {2, 1}, {3, 1}, {3, 2} } },
				{ ETetriminoFacing::West, { {1, 1}, {2, 0}, {2, 1}, {3, 0} } }
			},
			TEXT("/Game/Material/M_MinoMaterial_Red"),
			FIntVector2(ABoard::TetriminoDefaultSpawnLocationX - 2, ABoard::TetriminoDefaultSpawnLocationY)                               // FIntPoint
		}
	}
};

ATetrimino::ATetrimino()
	: TetriminoShape(ETetriminoShape::None)
	, Facing(ETetriminoFacing::North)
	, MatrixLocation(FIntVector2(0, 0))
	, MinoClass(AMino::StaticClass())
	, MinoArray()
	, bIsGhostPieceOn(true)
	, GhostPiece(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void ATetrimino::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATetrimino::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATetrimino::Initialize(const ETetriminoShape NewTetriminoShape)
{
	SetTetriminoShape(NewTetriminoShape);

	const FTetriminoInfo& TetriminoInfo = ATetrimino::GetTetriminoInfoByShape(TetriminoShape);

	InitializeMinos(TetriminoInfo);
	MatrixLocation = TetriminoInfo.InitialMatrixLocation;
}

void ATetrimino::MoveBy(const FIntVector2& IntVector2D)
{
	const FVector LocalOffset(AMino::Get3DRelativePositionByIntVector2D(IntVector2D));
	AddActorLocalOffset(LocalOffset);
	//DebugPrintState();
}

void ATetrimino::RotateBy(const int32 Value)
{
	SetFacing(Facing + Value);
	UpdateMinoPositions();
}

void ATetrimino::AttachToBoard(ABoard* const Board)
{
	AttachToActor(Board, FAttachmentTransformRules::KeepRelativeTransform);
	MoveBy(MatrixLocation);
	UE_LOG(LogTemp, Display, TEXT("MatrixLocation: (%d, %d)"), MatrixLocation.X, MatrixLocation.Y);
}

void ATetrimino::InitializeMinos(const FTetriminoInfo& TetriminoInfo)
{
	UMaterialInterface* const MinoMaterial = GetMaterialByTetriminoInfo(TetriminoInfo);
	if (!MinoMaterial)
	{
		return;
	}

	const TArray<FIntVector2>& MinoUnitPositions = TetriminoInfo.GetMinoUnitPosition(Facing);
	check(MinoUnitPositions.Num() == MinoNum);

	MinoArray.Reserve(MinoNum);
	for (int32 MinoID = 0; MinoID < MinoNum; ++MinoID)
	{
		const FIntVector2& MinoUnitPosition = MinoUnitPositions[MinoID];
		if (AMino* const Mino = GetWorld()->SpawnActor<AMino>(MinoClass, FVector::ZeroVector, FRotator::ZeroRotator))
		{
			Mino->SetMaterial(MinoMaterial);

			Mino->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			Mino->SetRelativeLocationByIntVector2D(MinoUnitPosition);

			MinoArray.Add(Mino);
		}
	}
}

void ATetrimino::UpdateMinoPositions()
{
	const FTetriminoInfo& TetriminoInfo = ATetrimino::GetTetriminoInfoByShape(TetriminoShape);

	UE_LOG(LogTemp, Warning, TEXT("Facing: %d, %s"), static_cast<int32>(Facing), *GetFacingName(Facing));
	check(0 <= static_cast<int32>(Facing) && static_cast<int32>(Facing) <= TetriminoInfo.MinoUnitPositionsByFacing.Num());

	const TArray<FIntVector2>& MinoUnitPositions = TetriminoInfo.GetMinoUnitPosition(Facing);
	for (int32 MinoID = 0; MinoID < MinoNum; ++MinoID)
	{
		if (AMino* const Mino = MinoArray[MinoID])
		{
			check(MinoUnitPositions.IsValidIndex(MinoID));
			const FIntVector2& IntPoint = MinoUnitPositions[MinoID];
			Mino->SetRelativeLocationByIntVector2D(IntPoint);
		}
	}
}

void ATetrimino::DebugPrintState() const
{
	UE_LOG(LogTemp, Log, TEXT("TetriminoInPlay Type: %s"), *GetTetriminoShapeName(TetriminoShape));
	UE_LOG(LogTemp, Log, TEXT("Tetrimino: Location: %s"), *GetActorLocation().ToString());

	for (int32 Index = 0; Index < MinoNum; ++Index)
	{
		if (MinoArray[Index])
		{
			const FVector MinoRelativeLocation = MinoArray[Index]->GetRelativeLocation();
			UE_LOG(LogTemp, Log, TEXT("Mino %d: Relative Location: %s"), Index, *MinoRelativeLocation.ToString());
		}
	}
}

const FTetriminoInfo& ATetrimino::GetTetriminoInfoByShape(const ETetriminoShape TetriminoShape)
{
	const FTetriminoInfo* TetriminoInfo = TetriminoInfos.Find(TetriminoShape);
	check(TetriminoInfo != nullptr);
	return *TetriminoInfo;
}

UMaterialInterface* ATetrimino::GetMaterialByTetriminoInfo(const FTetriminoInfo& TetriminoInfo)
{
	UMaterialInterface* const MinoMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *TetriminoInfo.MaterialPath));
	if (!MinoMaterial)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load material: %s"), *TetriminoInfo.MaterialPath);
	}
	return MinoMaterial;
}

FString ATetrimino::GetTetriminoShapeName(const ETetriminoShape TetriminoShape)
{
	static const TMap<ETetriminoShape, FString> TetriminoShapeNames =
	{
		{ETetriminoShape::O, TEXT("O")},
		{ETetriminoShape::I, TEXT("I")},
		{ETetriminoShape::T, TEXT("T")},
		{ETetriminoShape::L, TEXT("L")},
		{ETetriminoShape::J, TEXT("J")},
		{ETetriminoShape::S, TEXT("S")},
		{ETetriminoShape::Z, TEXT("Z")}
	};

	if (const FString* Name = TetriminoShapeNames.Find(TetriminoShape))
	{
		return *Name;
	}
	return TEXT("None");
}

FString ATetrimino::GetFacingName(const ETetriminoFacing Facing)
{
	static const TMap<ETetriminoFacing, FString> TetriminoFacingNames =
	{
		{ETetriminoFacing::North, TEXT("North")},
		{ETetriminoFacing::East, TEXT("East")},
		{ETetriminoFacing::South, TEXT("South")},
		{ETetriminoFacing::West, TEXT("West")},
	};

	if (const FString* Name = TetriminoFacingNames.Find(Facing))
	{
		return *Name;
	}
	return TEXT("None");
}

ETetriminoShape ATetrimino::GetTetriminoShapeRandom()
{
	const int32 RandomIndex = FMath::RandRange(0, static_cast<int32>(ETetriminoShape::Max) - 1);
	const ETetriminoShape TetriminoShape = static_cast<ETetriminoShape>(RandomIndex);
	return TetriminoShape;
}

