// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MyCharacterStatComponent.h"
#include "MyPlayerState.h"


void UMyHUDWidget::BindCharacterStat(UMyCharacterStatComponent* CharacterStat)
{
	CurrentCharacterStat = CharacterStat;
	CharacterStat->OnHPChanged.AddUObject(this, &UMyHUDWidget::UpdateCharacterStat);
	CharacterStat->OnFogGaugeChanged.AddUObject(this, &UMyHUDWidget::UpdateCharacterStat);
	UpdateCharacterStat();
}

void UMyHUDWidget::BindPlayerState(AMyPlayerState* PlayerState)
{
	CurrentPlayerState = PlayerState;
	PlayerState->OnPlayerStateChanged.AddUObject(this, &UMyHUDWidget::UpdatePlayerState);
}

void UMyHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));

	FogGaugeBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbFogGauge")));

	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerName")));

	PlayerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLevel")));

}

void UMyHUDWidget::UpdateCharacterStat()
{

	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
	FogGaugeBar->SetPercent(CurrentCharacterStat->GetFogGaugeRatio());
}

void UMyHUDWidget::UpdatePlayerState()
{
	PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));
	PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
}