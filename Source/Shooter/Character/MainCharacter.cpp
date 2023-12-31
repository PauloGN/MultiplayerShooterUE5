// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"
//Basic set up
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
//HUD
#include "Components/WidgetComponent.h"
//Replication
#include "Net/UnrealNetwork.h"
//Game Obj
#include "Shooter/Weapon/Weapon.h"
//Game Component
#include "Shooter/CharComponents/CombatComponent.h"
//Enhanced Input system
#include "EnhancedInputComponent.h"
#include <EnhancedInputSubsystems.h>

//#include "gmock/gmock-matchers.h"

AMainCharacter::AMainCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	cameraBoom->SetupAttachment(GetMesh());//attach to the mesh because in future root(capsule) will change size
	cameraBoom->TargetArmLength = 600.0f;
	cameraBoom->bUsePawnControlRotation = true;//rotate camera boom along with controller
	cameraBoom->SetWorldLocation({ 0.0f, 0.0f, 85.0f });

	followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	followCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	followCamera->bUsePawnControlRotation = false;

	//Controls rotation and movement
	bUseControllerRotationYaw = false;//do not rotata along with the controller rotation
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//HUD
	overheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidget"));
	overheadWidget->SetupAttachment(RootComponent);

	//Components
	combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	combat->SetIsReplicated(true);//Components does not need to be registered
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//New InputSystem
	//https://www.youtube.com/watch?v=E76KSs5ZoME&ab_channel=RogueEntity
	//https://www.youtube.com/watch?v=fW1pXOAIviw&t=3s&ab_channel=DruidMechanics stephan
	//https://www.youtube.com/watch?v=dS5AUaYFcdw C++
	//https://www.youtube.com/watch?v=_pPCS12aVtg&list=PLzykqv-wgIQXompUswD5iHllUHxGY7w0q
	if(UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(inputToJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		Input->BindAction(inputToMove, ETriggerEvent::Triggered, this, &ThisClass::EnhancedMove);
		Input->BindAction(inputLookUp, ETriggerEvent::Triggered, this, &ThisClass::EnhancedLook);
		Input->BindAction(inputAction, ETriggerEvent::Started, this, &ThisClass::EquipButtonPressed);
	}
}

void AMainCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if(combat)
	{
		combat->character = this;
	}

}

void AMainCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AMainCharacter, overlappingWeapon, COND_OwnerOnly);//pawn owner
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Enhanced system access add Input Mapping context
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			EnhancedInputSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AMainCharacter::EnhancedMove(const FInputActionValue& value)
{
	const FVector2D moveVector = value.Get<FVector2D>();

	//FRotator order
		//Y = Pitch
		//Z = Yaw
		//X = Roll

	//Move Forward
	if (Controller && moveVector.X != 0.0f)
	{
		//Getting a controller forwardDirection
		const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));

		AddMovementInput(Direction, moveVector.X);
	}
	//Move Right
	if (Controller && moveVector.Y != 0.0f)
	{
		//Getting a controller Right Direction
		const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));

		AddMovementInput(Direction, moveVector.Y);
	}
}

void AMainCharacter::EnhancedLook(const FInputActionValue& value)
{
	const FVector2D lookVector = value.Get<FVector2D>();

	if(GetController())
	{
		//Turn
		AddControllerYawInput(lookVector.X);
		//Look Up
		AddControllerPitchInput(lookVector.Y);
	}
}

void AMainCharacter::EquipButtonPressed()
{
	if(combat && HasAuthority())
	{
		combat->EquipWeapon(overlappingWeapon);
	}
}
//not called on server it is replicated down to client only
void AMainCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeaponRHS)
{
	if (overlappingWeapon)
	{
		overlappingWeapon->ShowPickupWidget(true);//Client
	}
	if (LastWeaponRHS)
	{
		LastWeaponRHS->ShowPickupWidget(false);//Client
	}
}

void AMainCharacter::SetOverlappingWeapon(AWeapon* WeaponRHS)
{

	if (overlappingWeapon)
	{
		if (IsLocallyControlled())
		{
			overlappingWeapon->ShowPickupWidget(false);
		}
	}

	overlappingWeapon = WeaponRHS;
	if (IsLocallyControlled())//The player hosting the game on the server
	{
		if (overlappingWeapon)
		{
			overlappingWeapon->ShowPickupWidget(true);
		}
	}
}