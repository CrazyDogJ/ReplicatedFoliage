#include "ReplicatedFoliageEditor.h"

#include "ISettingsModule.h"
#include "ReplicatedFoliage/Public/ReplicatedFoliageSettings.h"

#define LOCTEXT_NAMESPACE "FReplicatedFoliageEditorModule"

void FReplicatedFoliageEditorModule::StartupModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "Replicated Foliage", LOCTEXT("RuntimeSettingsName", "Replicated Foliage"), LOCTEXT("RuntimeSettingsDescription", "Configure Replicated Foliage"), GetMutableDefault<UReplicatedFoliageSettings>());
	}
}

void FReplicatedFoliageEditorModule::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Replicated Foliage");
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FReplicatedFoliageEditorModule, ReplicatedFoliageEditor)