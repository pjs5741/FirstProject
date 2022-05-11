// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnimInstance.h"

UBossAnimInstance::UBossAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsDead = false;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Animations/Grux_Skeleton_Montage.Grux_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE2(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Animations/Rush_Attack.Rush_Attack"));
	if (ATTACK_MONTAGE2.Succeeded())
	{
		AttackMontage2 = ATTACK_MONTAGE2.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE3(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Animations/Howling.Howling"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage3 = ATTACK_MONTAGE3.Object;
	}
}

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn))return;
		if (!IsDead)
		{
			CurrentPawnSpeed = Pawn->GetVelocity().Size();
			auto Character = Cast<ACharacter>(Pawn);
			
		}

}

void UBossAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

void UBossAnimInstance::PlayAttackMontage2()
{

	Montage_Play(AttackMontage2, 1.0f);
}

void UBossAnimInstance::PlayAttackMontage3()
{
	Montage_Play(AttackMontage3, 1.0f);
}

void UBossAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UBossAnimInstance::AnimNotify_AttackHitCheck2()
{
	OnAttackHitCheck2.Broadcast();
}

void UBossAnimInstance::AnimNotify_AttackHitCheck3()
{
	OnAttackHitCheck3.Broadcast();
}