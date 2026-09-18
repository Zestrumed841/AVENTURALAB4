// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Plataforma.h"
#include "PlataformaIndestructible.generated.h"

/**
 * 
 */
UCLASS()
class AVENTURAUSFX022026L4_API APlataformaIndestructible : public APlataforma
{
	GENERATED_BODY()
	
public:

	APlataformaIndestructible();

	virtual void Tick(float DeltaTime) override;

	// Detectar cuando la plataforma choca con algo
	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plataforma")
	float VelocidadCaida;

};
