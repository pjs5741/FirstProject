// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterStatComponent.h"
#include "MyGameInstance.h"

// Sets default values for this component's properties
UMyCharacterStatComponent::UMyCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	Level = 1;
	// ...
}


// Called when the game starts
void UMyCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMyCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UMyCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void UMyCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (nullptr == MyGameInstance) return;

	CurrentStatData = MyGameInstance->GetMyCharacterData(NewLevel);
	if (CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
		SetFogGauge(CurrentStatData->MaxFogGauge);
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("Level %d data doesn't exist."), NewLevel);
	}
}

void UMyCharacterStatComponent::SetDamage(float NewDamage)
{
	SetHP(FMath::Clamp<float>(CurrentHP - (NewDamage+NewDamage* GetFogGaugeRatio()/10), 0.0f, CurrentStatData->MaxHP));
}


void UMyCharacterStatComponent::SetUseFogGauge(float NewFogGauge)
{
	SetFogGauge(FMath::Clamp<float>(CurrentFogGauge, 0.0f, CurrentStatData->MaxFogGauge));
}
void UMyCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

void UMyCharacterStatComponent::SetFogGauge(float NewFogGauge)
{
	CurrentFogGauge = NewFogGauge;
	OnFogGaugeChanged.Broadcast();
	
}

float UMyCharacterStatComponent::GetAttack()
{
	return CurrentStatData->Attack;
}

float UMyCharacterStatComponent::GetHPRatio()
{
	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}

float UMyCharacterStatComponent::GetFogGaugeRatio()
{
	return (CurrentStatData->MaxFogGauge < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentFogGauge / CurrentStatData->MaxFogGauge);
}

float UMyCharacterStatComponent::GetCurrentFogGauge()
{
	return CurrentFogGauge;
}