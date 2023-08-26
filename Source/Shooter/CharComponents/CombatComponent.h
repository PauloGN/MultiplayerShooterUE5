// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

//Forward declarations
class AWeapon;
class AMainCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	friend class AMainCharacter;

	void EquipWeapon(AWeapon* weaponToEquip);
protected:

	virtual void BeginPlay() override;

private:

	AMainCharacter* character;
	AWeapon* equippedWeapon;

public:	

		
};
