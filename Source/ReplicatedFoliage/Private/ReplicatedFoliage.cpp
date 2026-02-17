// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReplicatedFoliage.h"

#include "ISettingsModule.h"
#include "ReplicatedFoliageSettings.h"

#define LOCTEXT_NAMESPACE "FReplicatedFoliageModule"

void FReplicatedFoliageModule::StartupModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "Replicated Foliage", LOCTEXT("RuntimeSettingsName", "Replicated Foliage"), LOCTEXT("RuntimeSettingsDescription", "Configure Replicated Foliage"), GetMutableDefault<UReplicatedFoliageSettings>());
	}
}

void FReplicatedFoliageModule::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Replicated Foliage");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FReplicatedFoliageModule, ReplicatedFoliage)