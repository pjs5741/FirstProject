// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"


UMyAnimInstance::UMyAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
	IsSetWeapon = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Mannequin/Animations/UE4_Mannequin_Skeleton_Montage.UE4_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Rolling_MONTAGE(TEXT("/Game/Mannequin/Animations/Rolling_Montage.Rolling_Montage"));
	if (Rolling_MONTAGE.Succeeded())
	{
		RollingMontage = Rolling_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipWeapon_MONTAGE(TEXT("/Game/Mannequin/Animations/SetWeqpon_Montage.SetWeqpon_Montage"));
	if (EquipWeapon_MONTAGE.Succeeded())
	{
		EquipWeaponMontage = EquipWeapon_MONTAGE.Object;
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;
	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
	
}

void UMyAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

void UMyAnimInstance::PlayRollingMontage()
{
	Montage_Play(RollingMontage, 1.8f);
}

void UMyAnimInstance::PlayEquipWeaponMontage()
{
	Montage_Play(EquipWeaponMontage, 1.0f);
}

void UMyAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UMyAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}



void UMyAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UMyAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

bool UMyAnimInstance::GetSetWeapon(bool a)
{
	IsSetWeapon = a;
	return IsSetWeapon;
}