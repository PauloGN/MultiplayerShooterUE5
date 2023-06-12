// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "Runtime/Engine/Public/TimerManager.h"

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (displayText)
	{
		displayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetLocalRole(APawn* InPawn)
{
	ENetRole LocalRole = InPawn->GetLocalRole();
	FString Role;

	switch (LocalRole)
	{
	case ROLE_None:

		Role = FString("None");

		break;
	case ROLE_SimulatedProxy:

		Role = FString("Simulated");

		break;
	case ROLE_AutonomousProxy:

		Role = FString("Autonomous");

		break;
	case ROLE_Authority:

		Role = FString("Authority");

		break;
	case ROLE_MAX:

		Role = FString("ROLE_MAX");

		break;
	default:
		break;
	}

	FString LocalRoleString = FString::Printf(TEXT("Local Role: %s"), *Role);
	SetDisplayText(LocalRoleString);
}

void UOverheadWidget::ShowPlayerNetRemoteRole(APawn* InPawn)
{
	ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString Role;

	switch (RemoteRole)
	{
	case ROLE_None:

		Role = FString("None");

		break;
	case ROLE_SimulatedProxy:

		Role = FString("Simulated");

		break;
	case ROLE_AutonomousProxy:

		Role = FString("Autonomous");

		break;
	case ROLE_Authority:

		Role = FString("Authority");

		break;
	case ROLE_MAX:

		Role = FString("ROLE_MAX");

		break;
	default:
		break;
	}

	FString RemoteRoleString = FString::Printf(TEXT("Remote Role: %s"), *Role);
	SetDisplayText(RemoteRoleString);
}


#pragma region HANDLE_PLAYER_NAME 

void UOverheadWidget::ShowPlayerName(APawn* InPawn)
{

	if (InPawn == nullptr)
	{
		// Tratar o erro de ponteiro nulo
		return;
	}

	const APlayerState* PlayerState = InPawn->GetPlayerState();
	if (!PlayerState || !*PlayerState->GetPlayerName() && TotalTime < GetPlayerNameTimeout)
	{
		SetPlayerNameTimer(InPawn);
		return;
	}

	const FString PlayerName = InPawn->GetPlayerState()->GetPlayerName();
	SetDisplayText(PlayerName);

}

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}

void UOverheadWidget::SetPlayerNameTimer(APawn* InPawn)
{
	FTimerDelegate TryAgainDelegate;
	TryAgainDelegate.BindUFunction(this, FName("ShowPlayerName"), InPawn);
	GetWorld()->GetTimerManager().SetTimer(GetPlayerStateTimer, TryAgainDelegate, GetPlayerNameInterval, false, 0.1f);

	TotalTime += GetPlayerNameInterval;
}

#pragma endregion