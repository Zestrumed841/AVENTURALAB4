// Fill out your copyright notice in the Description page of Project Settings.


#include "PlataformaIndestructible.h"
#include "AventuraUSFX022026L4Pawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"


APlataformaIndestructible::APlataformaIndestructible()
{
	// Usar un cubo como plataforma
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshIndestructible(
		TEXT("/Engine/BasicShapes/Cube.Cube")
	);

	if (MeshIndestructible.Succeeded())
	{
		mallaPlataforma->SetStaticMesh(MeshIndestructible.Object);
	}

	// Material de la plataforma indestructible                           cambio de color	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialIndestructible(
		TEXT("Material'/Game/cube.cube'")
	);

	if (MaterialIndestructible.Succeeded())
	{
		mallaPlataforma->SetMaterial(0, MaterialIndestructible.Object);
	}

	// Tamaño de la plataforma
	mallaPlataforma->SetRelativeScale3D(
		FVector(1.3f, 1.3f, 0.4f)
	);

	// Velocidad de caida
	VelocidadCaida = 150.0f;

	// Activar colisiones
	if (mallaPlataforma)
	{
		mallaPlataforma->SetCollisionProfileName(TEXT("BlockAll"));

		// Permitir detectar impactos
		mallaPlataforma->SetNotifyRigidBodyCollision(true);

		// Cuando choque, ejecutar OnHit
		mallaPlataforma->OnComponentHit.AddDynamic(
			this,
			&APlataformaIndestructible::OnHit
		);
	}
}
void APlataformaIndestructible::Tick(float DeltaTime)
{
		// Obtener la posicion actual
		FVector PosicionActual = GetActorLocation();

		// Hacer que la plataforma caiga
		PosicionActual.X -= VelocidadCaida * DeltaTime;

		// Guardar informacion del choque
		FHitResult Hit;

		// Mover la plataforma respetando colisiones
		SetActorLocation(PosicionActual, true, &Hit);

		// Comprobar si hubo un choque
		if (Hit.IsValidBlockingHit())
		{
			// Ver si chocamos con el Paddle
			AAventuraUSFX022026L4Pawn* Paddle =
				Cast<AAventuraUSFX022026L4Pawn>(Hit.GetActor());

			if (Paddle != nullptr)
			{
				// Destruir esta PlataformaIndestructible
				Destroy();
			}
		}
	}


void APlataformaIndestructible::OnHit(
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

	// Comprobar si chocamos con el Paddle
	AAventuraUSFX022026L4Pawn* Paddle =
		Cast<AAventuraUSFX022026L4Pawn>(OtherActor);

	if (Paddle != nullptr)
	{
		// Destruir ESTA plataforma indestructible
		Destroy();
	}
}