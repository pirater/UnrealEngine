// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "GameplayDebuggerPrivatePCH.h"
#include "GameplayDebuggerExtension_HUD.h"
#include "GameplayDebuggerConfig.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"

FGameplayDebuggerExtension_HUD::FGameplayDebuggerExtension_HUD()
{
	bIsGameHUDEnabled = false;
	bAreDebugMessagesEnabled = false;
	bPrevDebugMessagesEnabled = GEngine && GEngine->bEnableOnScreenDebugMessages;
	bIsCachedDescriptionValid = false;

	const UGameplayDebuggerConfig* SettingsCDO = UGameplayDebuggerConfig::StaticClass()->GetDefaultObject<UGameplayDebuggerConfig>();

	const bool bHasHUDBinding = BindKeyPress(SettingsCDO->GameHUDKey.GetFName(), 
		SettingsCDO->bUseGameHUDModifiers ? SettingsCDO->GameHUDModifiers.CreateModifier() : FGameplayDebuggerInputModifier::None,
		this, &FGameplayDebuggerExtension_HUD::ToggleGameHUD);
	HudBindingIdx = bHasHUDBinding ? (GetNumInputHandlers() - 1) : INDEX_NONE;

	const bool bHasMessagesBinding = BindKeyPress(SettingsCDO->DebugMessagesKey.GetFName(),
		SettingsCDO->bUseDebugMessagesModifiers ? SettingsCDO->DebugMessagesModifiers.CreateModifier() : FGameplayDebuggerInputModifier::None,
		this, &FGameplayDebuggerExtension_HUD::ToggleDebugMessages);
	MessagesBindingIdx = bHasMessagesBinding ? (GetNumInputHandlers() - 1) : INDEX_NONE;
}

TSharedRef<FGameplayDebuggerExtension> FGameplayDebuggerExtension_HUD::MakeInstance()
{
	return MakeShareable(new FGameplayDebuggerExtension_HUD());
}

void FGameplayDebuggerExtension_HUD::OnActivated()
{
	SetGameHUDEnabled(false);
	SetDebugMessagesEnabled(false);
}

void FGameplayDebuggerExtension_HUD::OnDeactivated()
{
	SetGameHUDEnabled(true);
	SetDebugMessagesEnabled(bPrevDebugMessagesEnabled);
}

FString FGameplayDebuggerExtension_HUD::GetDescription() const
{
	if (!bIsCachedDescriptionValid)
	{
		FString Description;

		if (HudBindingIdx != INDEX_NONE)
		{
			Description = FString::Printf(TEXT("{%s}%s:{%s}HUD"),
				*FGameplayDebuggerCanvasStrings::ColorNameInput,
				*GetInputHandlerDescription(HudBindingIdx),
				bIsGameHUDEnabled ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled);
		}

		if (MessagesBindingIdx != INDEX_NONE)
		{
			if (Description.Len() > 0)
			{
				Description += FGameplayDebuggerCanvasStrings::SeparatorSpace;
			}

			Description += FString::Printf(TEXT("{%s}%s:{%s}DebugMessages"),
				*FGameplayDebuggerCanvasStrings::ColorNameInput,
				*GetInputHandlerDescription(MessagesBindingIdx),
				bAreDebugMessagesEnabled ? *FGameplayDebuggerCanvasStrings::ColorNameEnabled : *FGameplayDebuggerCanvasStrings::ColorNameDisabled);
		}

		CachedDescription = Description;
		bIsCachedDescriptionValid = true;
	}

	return CachedDescription;
}

void FGameplayDebuggerExtension_HUD::SetGameHUDEnabled(bool bEnable)
{
	APlayerController* OwnerPC = GetPlayerController();
	AHUD* GameHUD = OwnerPC ? OwnerPC->GetHUD() : nullptr;
	if (GameHUD)
	{
		GameHUD->bShowHUD = bEnable;
	}

	bIsGameHUDEnabled = bEnable;
	bIsCachedDescriptionValid = false;
}

void FGameplayDebuggerExtension_HUD::SetDebugMessagesEnabled(bool bEnable)
{
	if (GEngine)
	{
		GEngine->bEnableOnScreenDebugMessages = bEnable;
	}

	bAreDebugMessagesEnabled = bEnable;
	bIsCachedDescriptionValid = false;
}

void FGameplayDebuggerExtension_HUD::ToggleGameHUD()
{
	SetGameHUDEnabled(!bIsGameHUDEnabled);
}

void FGameplayDebuggerExtension_HUD::ToggleDebugMessages()
{
	SetDebugMessagesEnabled(!bAreDebugMessagesEnabled);
}