// Fill out your copyright notice in the Description page of Project Settings.


#include "PlataformaDestructible.h"
#include "AventuraUSFX022026L4Projectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"



APlataformaDestructible::APlataformaDestructible()
{
	// 
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshDestructible(
		TEXT("/Engine/BasicShapes/Cube.Cube")
	);

	if (MeshDestructible.Succeeded())
	{
		mallaPlataforma->SetStaticMesh(MeshDestructible.Object);
	}

	// Tamaño de la plataforma                               tamaño 
	mallaPlataforma->SetRelativeScale3D(
		FVector(1.3f, 1.3f, 0.4f)
	); 


	// Velocidad inicial de caida
	VelocidadCaida = 150.0f;

	// Activar colisiones
	if (mallaPlataforma)
	{
		mallaPlataforma->SetCollisionProfileName(TEXT("BlockAll"));
		// Permitir que Unreal avise cuando ocurre un choque
		mallaPlataforma->SetNotifyRigidBodyCollision(true);

		// Cuando choque, ejecutar nuestra funcion OnHit
		mallaPlataforma->OnComponentHit.AddDynamic(
			this,
			&APlataformaDestructible::OnHit
		);
	}
}

void APlataformaDestructible::Tick(float DeltaTime)
{
	// NO ponemos Super::Tick(DeltaTime)
	// porque no queremos usar el movimiento aleatorio
	// de la clase Plataforma original.

	FVector PosicionActual = GetActorLocation();

	// Hace que la plataforma caiga por el escenario
	PosicionActual.X -= VelocidadCaida * DeltaTime;

	// El true hace que respete las colisiones
	SetActorLocation(PosicionActual, true);
}
void APlataformaDestructible::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	// Comprobar que realmente existe el objeto que nos golpeo
	if (OtherActor == nullptr)
	{
		return;
	}

	// Comprobar si quien golpeo fue nuestra pelota
	if (OtherActor->IsA(AAventuraUSFX022026L4Projectile::StaticClass()))
	{
		// Destruir ESTA plataforma
		Destroy();
	}
}
