// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Foggy.h"
#include "Components/ActorComponent.h"
#include "MyCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnFogGaugeIsExcessDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnFogGaugeChangedDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FOGGY_API UMyCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetNewLevel(int32 NewLevel);
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);
	void SetFogGauge(float NewFogGauge);
	float GetAttack();
	float GetHPRatio();
	float GetFogGaugeRatio();
	float GetCurrentFogGauge();
	void SetUseFogGauge(float NewFogGauge);

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnFogGaugeIsExcessDelegate OnFogGaugeIsExcess;
	FOnHPChangedDelegate OnHPChanged;
	FOnFogGaugeChangedDelegate OnFogGaugeChanged;

private:
	struct FMyCharacterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Level;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AlllowPrivateAccess = true))
		float CurrentHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AlllowPrivateAccess = true))
		float CurrentFogGauge;
		
};
