// Copyright Epic Games, Inc. All Rights Reserve

#include "AventuraUSFX022026L4Projectile.h"
#include "PlataformaDestructible.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/StaticMesh.h"

AAventuraUSFX022026L4Projectile::AAventuraUSFX022026L4Projectile() 
{
	// Static reference to the mesh to use for the projectile
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));

	// Nueva apariencia de pelota
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(
		TEXT("/Engine/BasicShapes/Sphere.Sphere")
	);



	// Create mesh component for the projectile sphere
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
	ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);

	// Tamaño de la pelota
	ProjectileMesh->SetRelativeScale3D(FVector(0.35f, 0.35f, 0.35f));


	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");

	// Permitir detectar impactos
	ProjectileMesh->SetNotifyRigidBodyCollision(true);

	// Las pelotas no chocan entre ellas
	//ProjectileMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AAventuraUSFX022026L4Projectile::OnHit);		// set up a notification for when this component hits something
	RootComponent = ProjectileMesh;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;

	//cambiar de false a true para que rebote                                     muy importante
	ProjectileMovement->bShouldBounce = true;

	// Rebote fuerte, sin perder casi velocidad
	ProjectileMovement->Bounciness = 1.0f;
	ProjectileMovement->Friction = 0.0f;

	// Evita que deje de moverse por tener poca velocidad
	ProjectileMovement->BounceVelocityStopSimulatingThreshold = 0.0f;

	//sin gravedad
	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity

	ProjectileMovement->bAutoActivate = true;

	// Mantener la pelota dentro del plano del escenario
	ProjectileMovement->bConstrainToPlane = true;
	ProjectileMovement->SetPlaneConstraintNormal(FVector(0.f, 0.f, 1.f));

	// Die after 3 seconds by default                                        tiempo de vida del proyectil
	InitialLifeSpan = 0.0f;
}

void AAventuraUSFX022026L4Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	// Si la pelota golpea una PlataformaDestructible
	if (OtherActor != nullptr)
	{
		APlataformaDestructible* Plataforma =
			Cast<APlataformaDestructible>(OtherActor);

		if (Plataforma != nullptr)
		{
			Plataforma->Destroy();
			return;
		}
	}

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 20.0f, GetActorLocation());
	}
	// aqui va destroy para destruir no destruir las balas o proyecctil
	//Destroy();
}