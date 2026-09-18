// Copyright Epic Games, Inc. All Rights Reserved.

#include "AventuraUSFX022026L4GameMode.h"
#include "AventuraUSFX022026L4Pawn.h"
#include "Plataforma.h"
#include "PlataformaAerea.h"
#include "PlataformaTerrestre.h"
#include "PlataformaAcuatica.h"
#include "PlataformaSubterranea.h"

// Nuevas plataformas del ejercicio
#include "PlataformaDestructible.h"
#include "PlataformaIndestructible.h"

AAventuraUSFX022026L4GameMode::AAventuraUSFX022026L4GameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AAventuraUSFX022026L4Pawn::StaticClass();
}

void AAventuraUSFX022026L4GameMode::BeginPlay()
{
	Super::BeginPlay();

	FRotator Rotacion = FRotator(0.0f, 0.0f, 0.0f);
	FVector SpawnLocation = FVector(-950.0f, 50.0f, 200.0f);


	UWorld* World = GetWorld();

	SpawnLocation = FVector(-500.0f, 100.0f, 150.0f);
	APlataforma* plataformaActual;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 10; j++) {
			SpawnLocation.X = SpawnLocation.X + i * 100.0f;
			SpawnLocation.Y = SpawnLocation.Y + j
				* 50.0f;
			if (World != nullptr)
			{
				plataformaActual = World->SpawnActor<APlataformaAerea>(SpawnLocation, Rotacion);
				aPlataformas.Add(plataformaActual);
			}
		}
	}

	SpawnLocation = FVector(-800.0f, 100.0f, 150.0f);
	for (int i = 0; i < 6; i++) {
		SpawnLocation.Y = SpawnLocation.Y + i * 50.0f;
		if (World != nullptr)
		{
			plataformaActual = World->SpawnActor<APlataformaTerrestre>(SpawnLocation, Rotacion);
			aPlataformas.Add(plataformaActual);
		}
	}

	SpawnLocation = FVector(-1200.0f, -600.0f, 150.0f);

	for (int i = 0; i < 7; i++) {
		int TipoEnemigo = FMath::RandRange(0.0f, 4.0f);

		switch (TipoEnemigo)
		{
			case 0:
				SpawnLocation.Y = SpawnLocation.Y + i * 50.0f;
				if (World != nullptr)
				{
					plataformaActual = World->SpawnActor<APlataformaTerrestre>(SpawnLocation, Rotacion);
					aPlataformas.Add(plataformaActual);
				}
			
				break;
			case 1:
				SpawnLocation.Y = SpawnLocation.Y + i * 50.0f;
				if (World != nullptr)
				{
					plataformaActual = World->SpawnActor<APlataformaAerea>(SpawnLocation, Rotacion);
					aPlataformas.Add(plataformaActual);
				}
			break;
			case 2:
				SpawnLocation.Y = SpawnLocation.Y + i * 50.0f;
				if (World != nullptr)
				{
					plataformaActual = World->SpawnActor<APlataformaSubterranea>(SpawnLocation, Rotacion);
					aPlataformas.Add(plataformaActual);
				}
			break;
			case 3:
				SpawnLocation.Y = SpawnLocation.Y + i * 50.0f;
				if (World != nullptr)
				{
					plataformaActual = World->SpawnActor<APlataformaAcuatica>(SpawnLocation, Rotacion);
					aPlataformas.Add(plataformaActual);
				}
			break;

			default:
			break;
		}

	}


	GetWorldTimerManager().SetTimer(TimerEliminarPlataforma, this, &AAventuraUSFX022026L4GameMode::EliminarPlataforma, 0.3 , true);
	GetWorldTimerManager().SetTimer(TimerReposicionarPlataformas, this, &AAventuraUSFX022026L4GameMode::ReposicionarPlataformas, 5, false);
	GetWorldTimerManager().SetTimer(TimerGenerarPlataformas,this,&AAventuraUSFX022026L4GameMode::GenerarPlataformas,5.0f,true);
	/*
	for (int i = 0; i < 2; i++) {

		for (int j = 0; j < 5; j++) {
			SpawnLocation = FVector(-950.0f + (i * 200.0f), 50.0f + (j * 200.0f), 200.0f);
			if (World != nullptr)
			{
				World->SpawnActor<APlataforma>(SpawnLocation, Rotacion);
			
			}
		}
	}

	*/

}

void AAventuraUSFX022026L4GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAventuraUSFX022026L4GameMode::EliminarPlataforma()
{
	int32 Indice = FMath::RandRange(0, 200);


	if (aPlataformas.IsValidIndex(Indice))
	{
		if (IsValid(aPlataformas[Indice]))
		{
			aPlataformas[Indice]->Destroy();

			aPlataformas[Indice] = nullptr;
		}
	}

}

void AAventuraUSFX022026L4GameMode::ReposicionarPlataformas()
{
	
	FVector posicionAereos = FVector(0.0f, 0.0f, 200.0f);
	FVector posicionTerrestres = FVector(-200.0f, -200.0f, 200.0f);

	for (APlataforma* plataformaActual : aPlataformas)
	{
		// Si la plataforma fue destruida o ya no existe,
		// pasar a la siguiente
		if (!IsValid(plataformaActual))
		{
			continue;
		}

		if (plataformaActual->TipoPlataforma == ETipoPlataforma::PLATAFORMA_AEREA)
		{
			plataformaActual->SetActorLocation(posicionAereos);
		}

		if (plataformaActual->TipoPlataforma == ETipoPlataforma::PLATAFORMA_TERRESTRE)
		{
			plataformaActual->SetActorLocation(posicionTerrestres);
		}
		
	}
	
}

void AAventuraUSFX022026L4GameMode::GenerarPlataformas()
{
	// Posicion superior del Area 1
	FVector PosicionArea1 = FVector(1340.0f, -1335.0f, 350.0f);

	// Posicion superior del Area 2
	FVector PosicionArea2 = FVector(1340.0f, 160.0f, 350.0f);

	// Posicion superior del Area 3
	FVector PosicionArea3 = FVector(1340.0f, 1490.0f, 350.0f);

	// Obtener el mundo
	UWorld* World = GetWorld();

	// Rotacion normal
	FRotator Rotacion = FRotator(0.0f, 0.0f, 0.0f);

	// Crear una plataforma aleatoria en el Area 1
	if (World != nullptr)
	{
		// 0 = Destructible
		// 1 = Indestructible
		int TipoPlataforma = FMath::RandRange(0, 1);

		APlataforma* PlataformaNueva = nullptr;

		if (TipoPlataforma == 0)
		{
			// Crear PlataformaDestructible
			PlataformaNueva =
				World->SpawnActor<APlataformaDestructible>(
					PosicionArea1,
					Rotacion
				);
		}
		else
		{
			// Crear PlataformaIndestructible
			PlataformaNueva =
				World->SpawnActor<APlataformaIndestructible>(
					PosicionArea1,
					Rotacion
				);
		}

		// Dar el mismo tamaño a las dos
		if (PlataformaNueva != nullptr)
		{
			PlataformaNueva->SetActorScale3D(
				FVector(2.0f, 5.0f, 2.0f)
			);
		}

		// ---------------- AREA 2 ----------------

		int TipoPlataformaArea2 = FMath::RandRange(0, 1);

		APlataforma* PlataformaNuevaArea2 = nullptr;

		if (TipoPlataformaArea2 == 0)
		{
			PlataformaNuevaArea2 =
				World->SpawnActor<APlataformaDestructible>(
					PosicionArea2,
					Rotacion
				);
		}
		else
		{
			PlataformaNuevaArea2 =
				World->SpawnActor<APlataformaIndestructible>(
					PosicionArea2,
					Rotacion
				);
		}

		if (PlataformaNuevaArea2 != nullptr)
		{
			PlataformaNuevaArea2->SetActorScale3D(
				FVector(2.0f, 5.0f, 2.0f)
			);
		}

		// ---------------- AREA 3 ----------------

		int TipoPlataformaArea3 = FMath::RandRange(0, 1);

		APlataforma* PlataformaNuevaArea3 = nullptr;

		if (TipoPlataformaArea3 == 0)
		{
			PlataformaNuevaArea3 =
				World->SpawnActor<APlataformaDestructible>(
					PosicionArea3,
					Rotacion
				);
		}
		else
		{
			PlataformaNuevaArea3 =
				World->SpawnActor<APlataformaIndestructible>(
					PosicionArea3,
					Rotacion
				);
		}

		if (PlataformaNuevaArea3 != nullptr)
		{
			PlataformaNuevaArea3->SetActorScale3D(
				FVector(2.0f, 5.0f, 2.0f)
			);
		}

	} // cierra if (World != nullptr)

} // cierra GenerarPlataformas()

	