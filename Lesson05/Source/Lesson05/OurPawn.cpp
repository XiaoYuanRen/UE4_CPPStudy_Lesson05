// Fill out your copyright notice in the Description page of Project Settings.

#include "OurPawn.h"
#include "RunTime/Engine/Classes/Camera/CameraComponent.h"

// Sets default values
AOurPawn::AOurPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomComponent"));

	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurCamera->SetupAttachment(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));

	OurVisibleComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AOurPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOurPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	{
		float CurrentScale = OurVisibleComponent->GetComponentScale().X;
		if (bGrowing)
		{
			CurrentScale += DeltaTime;
		}
		else
		{
			CurrentScale -= DeltaTime * 0.5f;
		}
		CurrentScale = FMath::Clamp(CurrentScale, 1.0f, 2.0f);
		OurVisibleComponent->SetWorldScale3D(FVector(CurrentScale));
	}

	{
		if (!CurrentVelocity.IsZero())
		{
			FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
			SetActorLocation(NewLocation);
		}
	}

}

// Called to bind functionality to input
void AOurPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("GrowUp", IE_Pressed, this, &AOurPawn::StartGrowing);
	PlayerInputComponent->BindAction("GrowUp", IE_Released, this, &AOurPawn::StopGrowing);

	PlayerInputComponent->BindAxis("MoveForward", this, &AOurPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AOurPawn::MoveRight);
}

void AOurPawn::MoveForward(float AxisValue)
{
	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void AOurPawn::MoveRight(float AxisValue)
{
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void AOurPawn::StartGrowing()
{
	bGrowing = true;
}

void AOurPawn::StopGrowing()
{
	bGrowing = false;
}

