// Copyright Ryu KeunBeom, Inc. All Rights Reserved.


#include "Mino.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AMino::AMino()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MinoMesh"));
	RootComponent = MinoMesh;

	// �⺻ ť�� �޽ø� ����մϴ�. �ʿ��� ��� �̸� ������ �� �ֽ��ϴ�.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		MinoMesh->SetStaticMesh(CubeMesh.Object);
	}

	// ũ�⸦ ��Ʈ���� ��� ũ�⿡ �°� �����մϴ�.
	MinoMesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
}

// Called when the game starts or when spawned
void AMino::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AMino::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector AMino::GetRelativeLocation() const
{
	return MinoMesh->GetRelativeLocation();
}

void AMino::SetRelativeLocation(const FVector NewLocation)
{
	MinoMesh->SetRelativeLocation(NewLocation);
}

void AMino::SetMaterial(const int32 ElementIndex, UMaterialInterface* const Material)
{
	MinoMesh->SetMaterial(ElementIndex, Material);
}
