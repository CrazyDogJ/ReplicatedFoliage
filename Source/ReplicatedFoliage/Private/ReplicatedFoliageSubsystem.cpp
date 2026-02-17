// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicatedFoliageSubsystem.h"

#include "ReplicatedFoliageISM.h"
#include "ReplicatedFoliageManager.h"
#include "ReplicatedFoliageSettings.h"

void UReplicatedFoliageSubsystem::SetReplicatedFoliageManager_Client(AReplicatedFoliageManager* Manager)
{
	if (!ReplicatedFoliageManager)
	{
		ReplicatedFoliageManager = Manager;
	}

	for (const auto Ism : GetReplicatedFoliageIsmComps())
	{
		Ism->RestoreRemovedInstances();
	}
}

AReplicatedFoliageManager* UReplicatedFoliageSubsystem::GetOrSpawnReplicatedFoliageManager()
{
	if (ReplicatedFoliageManager)
	{
		return ReplicatedFoliageManager;
	}

	// Only server can spawn.
	if (GetWorld()->GetAuthGameMode())
	{
		// Get class from project settings.
		TSubclassOf<AReplicatedFoliageManager> ManagerClass;
		if (const auto Settings = GetMutableDefault<UReplicatedFoliageSettings>())
		{
			ManagerClass = Settings->GetReplicatedFoliageManagerClass();
		}
		// Spawn actor by provided class.
		const auto SpawnActor = GetWorld()->SpawnActor(ManagerClass);
		ReplicatedFoliageManager = Cast<AReplicatedFoliageManager>(SpawnActor);
		return ReplicatedFoliageManager;
	}

	return nullptr;
}

void UReplicatedFoliageSubsystem::RegisterIsmComponent(UReplicatedFoliageISM* IsmComp)
{
	ReplicatedIsmComponents.Add(IsmComp);
}

void UReplicatedFoliageSubsystem::UnregisterIsmComponent(const UReplicatedFoliageISM* IsmComp)
{
	ReplicatedIsmComponents.Remove(IsmComp);
}

TSet<UReplicatedFoliageISM*> UReplicatedFoliageSubsystem::GetReplicatedFoliageIsmComps() const
{
	return ReplicatedIsmComponents;
}
