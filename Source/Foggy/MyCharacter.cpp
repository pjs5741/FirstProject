// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyAnimInstance.h"
#include "MyWeapon.h"
#include "MyCharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "MyCharacterWidget.h"
#include "MyAIController.h"
#include "MyPlayerController.h"
#include "GameFramework/Actor.h"
#include "MyGameInstance.h"
#include "MyPlayerController.h"
#include "MyPlayerState.h"
#include "MyHUDWidget.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UMyCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());


	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SKELETALMESH(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
	if (SKELETALMESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SKELETALMESH.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> BP_Character_C(TEXT("/Game/Mannequin/Animations/AnimBlueprint.AnimBlueprint_C"));
	if (BP_Character_C.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BP_Character_C.Class);
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/Foggy/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	GetCharacterMovement()->JumpZVelocity = 300.0f;
	SetControlMode(EControlMode::Basic);
	IsAttacking = false;
	IsRolling = false;
	IsEquip = false;
	MaxCombo = 2;
	AttackEndComboState();
	AttackRange = 80.0f;
	AttackRadius = 50.0f;
	bool CanBeDamaged(true);
	DeadTimer = 5.0f;



	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyCharacter"));
	WeaponItemClass = AMyWeapon::StaticClass();

	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto CharacterWidget = Cast<UMyCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
	bIsPlayer = IsPlayerControlled();
	if (bIsPlayer)
	{
		MyPlayerController = Cast<AMyPlayerController>(GetController());
	}
	else
	{
		MyAIController = Cast<AMyAIController>(GetController());
	}

	
	
	SetCharacterState(ECharacterState::READY);
}

void AMyCharacter::SetCharacterState(ECharacterState NewState)
{
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::READY:
	{
		auto MyPlayerState = Cast<AMyPlayerState>(GetPlayerState());
		CharacterStat->SetNewLevel(MyPlayerState->GetCharacterLevel());
		MyPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);


		CharacterStat->OnHPIsZero.AddLambda([this]()->void {
			SetCharacterState(ECharacterState::DEAD);
			});

		auto CharacterWidget = Cast<UMyCharacterWidget>(HPBarWidget->GetUserWidgetObject());
		CharacterWidget->BindCharacterStat(CharacterStat);

		if (bIsPlayer)
		{
			SetControlMode(EControlMode::Basic);
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			EnableInput(MyPlayerController);
		}
		else
		{
			SetControlMode(EControlMode::NPC);
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
			MyAIController->RunAI();
		}
		break;
	}
	case ECharacterState::DEAD:
	{
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(true);
		MyAnim->SetDeadAnim();
		SetCanBeDamaged(false);

		if (bIsPlayer)
		{
			DisableInput(MyPlayerController);
		}
		else
		{
			MyAIController->StopAI();
		}

		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {

			if (bIsPlayer)
			{
				MyPlayerController->RestartLevel();
			}
			else
			{
				Destroy();
			}
			}), DeadTimer, false);

		break;
	}
	}
}

ECharacterState AMyCharacter::GetCharacterState() const
{
	return CurrentState;
}

bool AMyCharacter::CanSetWeapon()
{
	return (nullptr == CurrentWeapon);
}

void AMyCharacter::SetWeapon(AMyWeapon* NewWeapon)
{
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}
// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Rolling"), EInputEvent::IE_Pressed, this, &AMyCharacter::Rolling);
	PlayerInputComponent->BindAction(TEXT("EquipWeapon"), EInputEvent::IE_Pressed, this, &AMyCharacter::EquipWeapon);
}

void AMyCharacter::MoveForward(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}

void AMyCharacter::MoveRight(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}

void AMyCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void AMyCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void AMyCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case EControlMode::Basic:
		SpringArm->TargetArmLength = 700.0f;
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::NPC:
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	}
}

void AMyCharacter::Attack()
{
	if (IsAttacking||IsRolling||IsEquip)
	{
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
		AttackStartComboState();
		MyAnim->PlayAttackMontage();
		MyAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void AMyCharacter::Rolling()
{

	if (IsRolling||IsAttacking|| IsEquip) return;

	

	MyAnim->PlayRollingMontage();
	IsRolling = true;
	SetCanBeDamaged(false);
}

void AMyCharacter::EquipWeapon()
{
	if (IsEquip ||IsAttacking || IsRolling) return;

	else if (nullptr != WeaponItemClass)
			{
				GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
				GetCharacterMovement()->MaxWalkSpeed = 0.0f;
				MyAnim->GetSetWeapon(true);
				IsEquip = true;
				MyAnim->PlayEquipWeaponMontage();
				if (CanSetWeapon())
				{
					auto NewWeapon = GetWorld()->SpawnActor<AMyWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
					SetWeapon(NewWeapon);
				}
				else if (CanSetWeapon() == false)
				{
					MyAnim->GetSetWeapon(false);
					CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					CurrentWeapon->Destroy();
					CurrentWeapon = nullptr;
				}
			}
	
	
}
void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MyAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());


	MyAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
	MyAnim->OnMontageEnded.AddDynamic(this,  &AMyCharacter::OnRollingMontageEnded);
	MyAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnEquipWeaponMontageEnded);

	MyAnim->OnNextAttackCheck.AddLambda([this]()->void {
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			MyAnim->JumpToAttackMontageSection(CurrentCombo);
		}
		});
	MyAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck);

	CharacterStat->OnHPIsZero.AddLambda([this]()->void {
		MyAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		});
	
}

void AMyCharacter::OnRollingMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsRolling = false;
	SetCanBeDamaged(true);
}

void AMyCharacter::OnEquipWeaponMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	IsEquip = false;
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	IsAttacking = false;
	AttackEndComboState();
	OnAttackEnd.Broadcast();
}


void AMyCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AMyCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AMyCharacter::AttackCheck()
{
	float FinalAttackRange = GetFinalAttackRange();
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * FinalAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

/*
#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * FinalAttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = FinalAttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif
*/
	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(CharacterStat->GetAttack()+CharacterStat->GetAttack()*CurrentCombo/10, DamageEvent, GetController(), this);
		}
	}

}

float AMyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CharacterStat->SetDamage(FinalDamage);
	return FinalDamage;
}


void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		SetControlMode(EControlMode::Basic);
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else
	{
		SetControlMode(EControlMode::NPC);
		GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	}
}

float AMyCharacter::GetFinalAttackRange() const
{
	return(nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackRange() : AttackRange;
}