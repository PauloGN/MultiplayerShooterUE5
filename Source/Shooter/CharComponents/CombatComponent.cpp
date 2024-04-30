// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"

#include "Engine/SkeletalMeshSocket.h"
#include "Shooter/Character/MainCharacter.h"
#include "Shooter/Weapon/Weapon.h"

UCombatComponent::UCombatComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::EquipWeapon(AWeapon* weaponToEquip)
{
	if(character == nullptr || weaponToEquip == nullptr)
	{
		return;
	}

	equippedWeapon = weaponToEquip;
	equippedWeapon->SetWeaponState(EWeaponState::Ews_Equipped);

	auto* HandSocket = character->GetMesh()->GetSocketByName("RightHandSocket");
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "AEWWWWQQQ");
	if(HandSocket)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, "AEWWWWQQQ");
		HandSocket->AttachActor(equippedWeapon, character->GetMesh());
		equippedWeapon->ShowPickupWidget(false);
	}

	equippedWeapon->SetOwner(character);
}