// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AventuraUSFX022026L4GameMode.generated.h"

class APlataforma;

UCLASS(MinimalAPI)
class AAventuraUSFX022026L4GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAventuraUSFX022026L4GameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	TArray<APlataforma*> aPlataformas;
	FTimerHandle TimerEliminarPlataforma;
	FTimerHandle TimerReposicionarPlataformas;

	// Timer para crear plataformas cada 5 segundos
	FTimerHandle TimerGenerarPlataformas;

	void EliminarPlataforma();
	void ReposicionarPlataformas();

	// Genera plataformas aleatorias en las 3 areas
	void GenerarPlataformas();
};

