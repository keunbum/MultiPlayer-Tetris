/**
 * @file Tetrimino.h
 * @brief Defines the ATetrimino class, which represents a tetrimino in the game.
 */

#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "GameFramework/Actor.h"
#include "Math/MathFwd.h"

#include "EnumClassOperators.h"

#include "Tetrimino.generated.h"

class UMino;
class ABoard;
class UMino;
class UMaterialInterface;

UENUM()
enum class ETetriminoShape : int8
{
	None = -1,
	O,
	I,
	T,
	L,
	J,
	S,
	Z,
	Max
};

UENUM()
enum class ETetriminoFacing : uint8
{
	North,
	East,
	South,
	West,
	Max
};

ENUM_CLASS_OPERATORS(ETetriminoFacing)

UENUM()
enum class ETetriminoRotationDirection : int8
{
	CounterClockwise = -1,
	Clockwise = 1
};

struct FTetriminoShapeInfo
{
	TMap<ETetriminoFacing, TArray<FIntPoint>> MinoMatrixLocalLocationsByFacing;
	FString MaterialPath;
	FLinearColor Color;
	FIntPoint InitialMatrixLocation;
	TMap<ETetriminoFacing, TMap<ETetriminoRotationDirection, TArray<FIntPoint>>> SRSRotationPointOffsetsTable; // Super Rotation System Rotation Point Table
};

/**
 * @class ATetrimino
 * @brief Represents a tetrimino in the game.
 */
UCLASS()
class MULTIPLAYERTETRIS_API ATetrimino : public AActor
{
	GENERATED_BODY()

public:
	ATetrimino();
	virtual void Tick(const float DeltaTime) override;

	const FTetriminoShapeInfo& GetTetriminoShapeInfo() const;
	const FIntPoint& GetInitialMatrixLocation() const;
	const TArray<FIntPoint>& GetMinoMatrixLocalLocations() const;
	const TArray<FIntPoint>& GetSRSRotationPointOffsets(const ETetriminoRotationDirection RotationDirection) const;

	const ETetriminoShape& GetShape() const { return Shape; }
	const ETetriminoFacing& GetFacing() const { return Facing; }
	const FIntPoint& GetMatrixLocation() const { return MatrixLocation; }
	const TArray<TObjectPtr<UMino>>& GetMinoArray() const { return MinoArray; }

	void Initialize(const ETetriminoShape NewTetriminoShape);
	void SetShape(const ETetriminoShape NewShape) { Shape = NewShape; }
	void MoveBy(const FIntPoint& IntVector2D);
	void RotateTo(const ETetriminoRotationDirection RotationDirection);
	void AttachToBoard(ABoard* const Board);

	void DebugPrintState() const;

	static ETetriminoShape GetTetriminoShapeRandom();
	static const TArray<FIntPoint>& GetMinoMatrixLocalLocationsByTetriminoShapeAndFacing(const ETetriminoShape Shape, const ETetriminoFacing Facing);

protected:
	virtual void BeginPlay() override;

	void SetFacing(const ETetriminoFacing NewFacing) { Facing = NewFacing; }

	void InitializeMinoArray();
	void UpdateMinoMatrixLocalLocations();

	static const FTetriminoShapeInfo& GetTetriminoShapeInfoByShape(const ETetriminoShape Shape);
	static const FIntPoint& GetInitialMatrixLocationByShape(const ETetriminoShape Shape);

	struct FRotationInfo
	{
		ETetriminoShape Shape;
		ETetriminoFacing Facing;
		ETetriminoRotationDirection Direction;
	};
	static const TArray<FIntPoint>& GetSRSRotationPointOffsetsByRotationInfo(const FRotationInfo& RotationInfo);
	static FString GetTetriminoShapeName(const ETetriminoShape Shape);
	static FString GetFacingName(const ETetriminoFacing Facing);

public:
	static constexpr int32 MinoNum = 4;
	static const FVector2D MoveDirectionLeft;
	static const FVector2D MoveDirectionRight;
	static const FVector2D MoveDirectionDown;

	static const TMap<ETetriminoShape, FTetriminoShapeInfo> TetriminoShapeInfos;

private:
	UPROPERTY(VisibleAnywhere)
	ETetriminoShape Shape;

	UPROPERTY(VisibleAnywhere)
	ETetriminoFacing Facing;

	UPROPERTY(VisibleAnywhere)
	FIntPoint MatrixLocation;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UMino> MinoClass;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UMino>> MinoArray;

	UPROPERTY(EditDefaultsOnly)
	bool bIsGhostPieceOn;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATetrimino> GhostPiece;
};
