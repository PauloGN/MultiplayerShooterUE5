// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"

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
