// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Foggy.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FMyCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMyCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f),MaxFogGauge(100.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxFogGauge;
	

};
/**
 * 
 */
UCLASS()
class FOGGY_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UMyGameInstance();

	FMyCharacterData* GetMyCharacterData(int32 Level);

private:
	UPROPERTY()
		class UDataTable* MyCharacterTable;
};
