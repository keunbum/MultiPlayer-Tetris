// Copyright Ryu KeunBeom. All Rights Reserved.

#include "Tetrimino.h"

#include "GhostPiece.h"
#include "Board.h"
#include "Mino.h"

ATetrimino::ATetrimino()
	: GhostPiece(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

const FMinoInfo ATetrimino::GetMinoInfo() const
{
	const FTetriminoShapeInfo& ShapeInfo = GetTetriminoShapeInfo();
	return FMinoInfo(ShapeInfo.MaterialPath, ShapeInfo.Color, ATetrimino::Opacity, ATetrimino::TranslucentSortPriority);
}

void ATetrimino::SetGhostPiece(AGhostPiece* const InGhostPiece)
{
	GhostPiece = InGhostPiece;
	if (GhostPiece)
	{
		const ATetriminoBase::FInitializeParams InitializeParams(Shape, Facing);
		GhostPiece->Initialize(InitializeParams);
		check(Board != nullptr);
		GhostPiece->SetRelativeLocationByMatrixLocation(Board->GetFinalFallingMatrixLocation(this));
	}
}

void ATetrimino::MoveBy(const FIntPoint& IntPoint2D)
{
	AddRelativeLocationByMatrixLocationOffset(IntPoint2D);

	if (GhostPiece)
	{
		check(Board != nullptr);
		GhostPiece->SetRelativeLocationByMatrixLocation(Board->GetFinalFallingMatrixLocation(this));
	}
}

void ATetrimino::RotateTo(const ETetriminoRotationDirection RotationDirection)
{
	const ETetriminoFacing NewFacing = Facing + static_cast<int32>(RotationDirection);

	RotateByFacing(NewFacing);
	if (GhostPiece)
	{
		GhostPiece->RotateByFacing(NewFacing);
	}
}

void ATetrimino::SetBoard(ABoard* const InBoard)
{
	Board = InBoard;
	if (Board)
	{
		AttachToMatrix(Board->GetMatrixRoot());
	}
	else
	{
		DetachFromMatrix();
	}
}

void ATetrimino::AttachToMatrix(USceneComponent* const MatrixRoot)
{
	check(MatrixRoot != nullptr);
	AttachToComponentByMatrixLocation(MatrixRoot, GetInitialMatrixLocation());
}

void ATetrimino::DetachFromMatrix()
{
	RootComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
}
