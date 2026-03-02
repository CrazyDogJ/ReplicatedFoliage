// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicatedFoliageManager.h"

#include "ReplicatedFoliageISM.h"
#include "ReplicatedFoliageSubsystem.h"
#include "Net/UnrealNetwork.h"

AReplicatedFoliageManager::AReplicatedFoliageManager()
{
	PrimaryActorTick.bCanEverTick = false;
#if WITH_EDITOR
	bIsSpatiallyLoaded = false;
#endif
	bReplicates = true;
	bAlwaysRelevant = true;

	ReplicatedArrayState = FFoliageInstanceStateArray(this);
}

void AReplicatedFoliageManager::PostGameLoaded()
{
	ReplicatedArrayState.MarkArrayDirty();
	
	const auto Subsystem = GetWorld()->GetSubsystem<UReplicatedFoliageSubsystem>();
	const auto Isms = Subsystem->GetReplicatedFoliageIsmComps();
	
	for (const auto Ism : Isms)
	{
		Ism->RestoreRemovedInstances();
	}
}

void AReplicatedFoliageManager::PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize)
{
}

void AReplicatedFoliageManager::PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize)
{
	for (const auto Index : AddedIndices)
	{
		CheckRemoveInstance(ReplicatedArrayState.Items[Index]);
	}
}

void AReplicatedFoliageManager::PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize)
{
	for (const auto Index : ChangedIndices)
	{
		CheckRemoveInstance(ReplicatedArrayState.Items[Index]);
	}
}

void AReplicatedFoliageManager::InstanceGetDamaged(UReplicatedFoliageISM* IsmPtr, int InstanceIndex, float Damage, FVector Direction)
{
	const auto Found = ReplicatedArrayState.Items.FindByPredicate(
		[IsmPtr, InstanceIndex](const FFoliageInstanceStateEntry& Entry)
	{
		return Entry.InstanceComponentGuid == IsmPtr->IdentityId && Entry.InstanceIndex == InstanceIndex;
	});

	if (Found)
	{
		Found->InstanceFloat -= Damage;
		ReplicatedArrayState.MarkItemDirty(*Found);
		// Listen server update
		CheckRemoveInstance(*Found, Direction);
		return;
	}

	// TODO: Make every tree has different health point.
	const float MaxHealth = 100.0f;
	
	auto NewEntry = FFoliageInstanceStateEntry();
	NewEntry.InstanceComponentGuid = IsmPtr->IdentityId;
	NewEntry.InstanceIndex = InstanceIndex;
	NewEntry.InstanceFloat = Damage >= MaxHealth ? 0.0f : MaxHealth - Damage;
	// TODO: Bool is not used for now.
	NewEntry.InstanceBool = false;
	
	ReplicatedArrayState.Items.Add(NewEntry);
	ReplicatedArrayState.MarkItemDirty(NewEntry);
	CheckRemoveInstance(NewEntry, Direction);
}

void AReplicatedFoliageManager::CheckRemoveInstance(const FFoliageInstanceStateEntry& Item, FVector Direction)
{
	// Check should remove instance.
	if (Item.InstanceFloat <= 0.0f)
	{
		RemoveInstance(Item.InstanceComponentGuid, Item.InstanceIndex, Direction);
	}
}

void AReplicatedFoliageManager::RemoveInstance(const FGuid CompGuid, const int Index, FVector Direction)
{
	const auto Subsystem = GetWorld()->GetSubsystem<UReplicatedFoliageSubsystem>();
	const auto Isms = Subsystem->GetReplicatedFoliageIsmComps();
	
	for (const auto Ism : Isms)
	{
		if (Ism->IdentityId == CompGuid)
		{
			// Get transform first.
			auto NewRemovedContext = FFoliageInstanceRemoveContext();
			Ism->GetInstanceTransform(Index, NewRemovedContext.Transform, true);
			NewRemovedContext.ForceDirection = Direction;
			
			Ism->RemoveInstance(Index);

			// Broadcast instance removed event.
			NewRemovedContext.StaticMesh = Ism->GetStaticMesh();
			Subsystem->OnFoliageInstanceRemovedEvent.Broadcast(NewRemovedContext);
			
			break;
		}
	}
}

void AReplicatedFoliageManager::BeginPlay()
{
	Super::BeginPlay();

	// Set replicated foliage manager in subsystem.
	auto Subsystem = GetWorld()->GetSubsystem<UReplicatedFoliageSubsystem>();
	Subsystem->SetReplicatedFoliageManager_Client(this);
}

void AReplicatedFoliageManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ReplicatedArrayState);
}
