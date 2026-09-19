// Copyright Epic Games, Inc. All Rights Reserved. si se preguntn porque la plataforma del carril 3 nose destruye es porque primero se debe destrir de frente con el paw para q desaparesaca y luego todo sera normal

#include "AventuraUSFX022026L4Pawn.h"
#include "AventuraUSFX022026L4Projectile.h"
#include "PlataformaIndestructible.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

const FName AAventuraUSFX022026L4Pawn::MoveForwardBinding("MoveForward");
const FName AAventuraUSFX022026L4Pawn::MoveRightBinding("MoveRight");
const FName AAventuraUSFX022026L4Pawn::FireForwardBinding("FireForward");
const FName AAventuraUSFX022026L4Pawn::FireRightBinding("FireRight");

AAventuraUSFX022026L4Pawn::AAventuraUSFX022026L4Pawn()
{	// Mesh original de la nave
// static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));

	// Mesh para el Paddle (cambio TwinStickUFO por Cube)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(
		TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'")
	);



	// Create the mesh component                               
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

	// Permitir detectar impactos del Paddle
	ShipMeshComponent->SetNotifyRigidBodyCollision(true);

	ShipMeshComponent->OnComponentHit.AddDynamic(
		this,
		&AAventuraUSFX022026L4Pawn::OnHit
	);


	// Dar forma rectangular al Paddle
	ShipMeshComponent->SetRelativeScale3D(FVector(0.45f, 7.0f, 0.20f));
	
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	// Movement
	MoveSpeed = 1000.0f;

	// Configuracion inicial del movimiento
	bMovimientoLibre = false;

	// Para nuestro Paddle solo permitimos Este y Oeste
	bPermitirNorte = false;
	bPermitirSur = false;
	bPermitirEste = true;
	bPermitirOeste = true; 

	// El Paddle no gira al moverse
	bRotarConMovimiento = false;

	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;
}

void AAventuraUSFX022026L4Pawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
}

void AAventuraUSFX022026L4Pawn::Tick(float DeltaSeconds)
{
	// Find movement direction
	 float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	 float RightValue = GetInputAxisValue(MoveRightBinding);

	 // Si NO esta activado el movimiento libre,
// controlamos cada direccion por separado.
	 if (!bMovimientoLibre)
	 {
		 // Norte
		 if (ForwardValue > 0.0f && !bPermitirNorte)
		 {
			 ForwardValue = 0.0f;
		 }

		 // Sur
		 if (ForwardValue < 0.0f && !bPermitirSur)
		 {
			 ForwardValue = 0.0f;
		 }

		 // Este
		 if (RightValue > 0.0f && !bPermitirEste)
		 {
			 RightValue = 0.0f;
		 }

		 // Oeste
		 if (RightValue < 0.0f && !bPermitirOeste)
		 {
			 RightValue = 0.0f;
		 }
	 }

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		FRotator NewRotation = GetActorRotation();

		if (bRotarConMovimiento)
		{
			NewRotation = Movement.Rotation();
		}

		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);
		
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}
	
	// Create fire direction vector
	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);

	// Try and fire a shot
	FireShot(FireDirection);
}

void AAventuraUSFX022026L4Pawn::FireShot(FVector FireDirection)
{
	// If it's ok to fire again
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				// spawn the projectile
				World->SpawnActor<AAventuraUSFX022026L4Projectile>(SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AAventuraUSFX022026L4Pawn::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	}
}

void AAventuraUSFX022026L4Pawn::ShotTimerExpired()
{
	bCanFire = true;
}

void AAventuraUSFX022026L4Pawn::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (OtherActor == nullptr)
	{
		return;
	}

	// Comprobar si el Paddle choco con una PlataformaIndestructible
	APlataformaIndestructible* Plataforma =
		Cast<APlataformaIndestructible>(OtherActor);

	if (Plataforma != nullptr)
	{
		// El Paddle destruye solamente la Indestructible ...
		Plataforma->Destroy();
	}
}
