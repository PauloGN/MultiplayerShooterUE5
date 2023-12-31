// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayere)
{
	Super::PostLogin(NewPlayere);


	int32 NumberOfPlayers =  GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers >= 4)
	{
		UWorld* World = GetWorld();

		if (World)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Maps/Laboratory?listen"));
		}
	}

}
