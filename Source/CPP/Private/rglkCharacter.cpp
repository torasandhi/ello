// Fill out your copyright notice in the Description page of Project Settings.


#include "rglkCharacter.h"

#include "Components/WeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ArglkCharacter::ArglkCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Configure standard movement for both AI and Player
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	WeaponComp = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
}

void ArglkCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void ArglkCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ArglkCharacter::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Base Character Attacking!"));
}

void ArglkCharacter::Die()
{
	// Ragdoll, destroy actor, etc.
}