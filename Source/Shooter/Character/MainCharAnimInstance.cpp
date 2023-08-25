// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharAnimInstance.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMainCharAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	mainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());

}

void UMainCharAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (mainCharacter == nullptr)
	{
		mainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	}

	if (mainCharacter == nullptr) return;

	FVector Velocity = mainCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = mainCharacter->GetCharacterMovement()->IsFalling();

	bIsAccelerating = (mainCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size()) > 0.0f ? true : false;
}
