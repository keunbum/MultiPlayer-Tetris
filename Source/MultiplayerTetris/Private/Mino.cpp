// Fill out your copyright notice in the Description page of Project Settings.


#include "Mino.h"

#include "UObject/ConstructorHelpers.h"

const FName UMino::BaseColorParameterName = TEXT("BaseColor");
const FString UMino::CubeMeshPath = TEXT("/Engine/BasicShapes/Cube.Cube");
TMap<FString, UMaterialInstanceDynamic*> UMino::MaterialCache;

UMino::UMino()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(*UMino::CubeMeshPath);
	if (CubeMesh.Succeeded())
	{
		SetStaticMesh(CubeMesh.Object);
	}

	SetWorldScale3D(FVector(MinoScale));
}

void UMino::SetRelativeLocationByMatrixLocation(const FIntPoint& MatrixLocation, const float Z)
{
	SetRelativeLocation(UMino::Get3DRelativePositionByMatrixLocation(MatrixLocation, Z));
}

UMino* UMino::CreateMino(UObject* const InOuter, USceneComponent* const Parent, const FMinoInfo& MinoInfo, const FIntPoint& MatrixLocation, const float Z)
{
	if (UMino* const Mino = NewObject<UMino>(InOuter))
	{
		static constexpr int32 ElementIndex = 0;
		Mino->SetMaterial(ElementIndex, UMino::GetMaterialInstanceByMinoInfo(InOuter, MinoInfo));
		Mino->RegisterComponent();
		Mino->AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform);
		Mino->SetRelativeLocationByMatrixLocation(MatrixLocation, Z);
		return Mino;
	}
	return nullptr;
}

void UMino::ClearMaterialCache()
{
	MaterialCache.Empty();
}

FVector UMino::Get3DRelativePositionByMatrixLocation(const FIntPoint& MatrixLocation, const float Z)
{
	const float X = -UnitLength * MatrixLocation.Y;
	const float Y = -UnitLength * MatrixLocation.X;
	return FVector(X, Y, Z);
}

UMaterialInterface* UMino::GetMaterialByMinoInfo(const FMinoInfo& MinoInfo)
{
	UMaterialInterface* MinoMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *MinoInfo.MaterialPath));
	ensureMsgf(MinoMaterial != nullptr, TEXT("Failed to load material: %s"), *MinoInfo.MaterialPath);
	return MinoMaterial;
}

UMaterialInstanceDynamic* UMino::GetMaterialInstanceByMinoInfo(UObject* const InOuter, const FMinoInfo& MinoInfo)
{
	check(!MinoInfo.MaterialPath.IsEmpty());

	const FString MaterialKey = MinoInfo.MaterialPath + MinoInfo.Color.ToString();

	if (UMaterialInstanceDynamic** const FoundMaterial = MaterialCache.Find(MaterialKey))
	{
		return *FoundMaterial;
	}

	if (UMaterialInterface* const BaseMaterial = UMino::GetMaterialByMinoInfo(MinoInfo))
	{
		if (UMaterialInstanceDynamic* const DynamicMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, InOuter))
		{
			DynamicMaterialInstance->SetVectorParameterValue(BaseColorParameterName, MinoInfo.Color);
			MaterialCache.Add(MaterialKey, DynamicMaterialInstance);
			return DynamicMaterialInstance;
		}
	}
	return nullptr;
}
