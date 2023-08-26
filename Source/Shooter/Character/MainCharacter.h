// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "MainCharacter.generated.h"

UCLASS()
class SHOOTER_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	//REPLICATION
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps)const override;

protected:

	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* cameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* followCamera;

	// HUD
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = HUD)
	class UWidgetComponent* overheadWidget;

	//WEAPON
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* overlappingWeapon;

	//Replication
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeaponRHS);

	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* combat;

public:	

	//Weapon
	void SetOverlappingWeapon(AWeapon* WeaponRHS);

//Input System
#pragma region ENHANCED INPUT SYSTEM

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	class UInputMappingContext* InputMappingContext;

	//Inputs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	class UInputAction* inputToJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* inputToMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* inputLookUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* inputAction;

protected:
	///Enhanced movement
	void EnhancedMove(const FInputActionValue& value);
	void EnhancedLook(const FInputActionValue& value);
	///
	///Enhanced Actions
	void EquipButtonPressed();
	///
#pragma endregion

};
