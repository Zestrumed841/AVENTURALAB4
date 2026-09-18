// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Plataforma.h"
#include "PlataformaDestructible.generated.h"

/**
 * 
 */
UCLASS()
class AVENTURAUSFX022026L4_API APlataformaDestructible : public APlataforma
{
	GENERATED_BODY()
	
public:

	// Constructor
	APlataformaDestructible();

	// Movimiento de la plataforma
	virtual void Tick(float DeltaTime) override;

	// Funcion que se ejecuta cuando algo choca con la plataforma
	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

	// Velocidad con la que cae
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plataforma")
	float VelocidadCaida;
};
