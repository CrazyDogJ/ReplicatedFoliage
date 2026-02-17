// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicatedFoliageISM.h"

#include "ReplicatedFoliageManager.h"
#include "ReplicatedFoliageSubsystem.h"

UReplicatedFoliageISM::UReplicatedFoliageISM()
{
	// Init id at construction.
	if (!IdentityId.IsValid())
	{
		IdentityId = FGuid::NewGuid();
	}
}

void UReplicatedFoliageISM::BeginPlay()
{
	Super::BeginPlay();

	// Create rep manager.
	const auto Subsystem = GetWorld()->GetSubsystem<UReplicatedFoliageSubsystem>();
	Subsystem->GetOrSpawnReplicatedFoliageManager();

	Subsystem->RegisterIsmComponent(this);

	// Only server can deal with damage
	// and restore at begin play
	if (GetOwner()->HasAuthority())
	{
		OnInstanceTakePointDamage.AddDynamic(this, &UReplicatedFoliageISM::ReceivePointDamage);
		OnInstanceTakeRadialDamage.AddDynamic(this, &UReplicatedFoliageISM::ReceiveRadialDamage);
	}

	RestoreRemovedInstances();
}

void UReplicatedFoliageISM::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	const auto Subsystem = GetWorld()->GetSubsystem<UReplicatedFoliageSubsystem>();
	Subsystem->UnregisterIsmComponent(this);
	
	// Only server can deal with damage.
	if (GetOwner()->HasAuthority())
	{
		OnInstanceTakePointDamage.RemoveDynamic(this, &UReplicatedFoliageISM::ReceivePointDamage);
		OnInstanceTakeRadialDamage.RemoveDynamic(this, &UReplicatedFoliageISM::ReceiveRadialDamage);
	}
}

void UReplicatedFoliageISM::ReceivePointDamage(int32 InstanceIndex, float Damage, class AController* InstigatedBy,
                                               FVector HitLocation, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
{
	const auto Subsystem = GetWorld()->GetSubsystem<UReplicatedFoliageSubsystem>();

	if (auto ReplicatedFoliageManager = Subsystem->GetOrSpawnReplicatedFoliageManager())
	{
		ReplicatedFoliageManager->InstanceGetDamaged(this, InstanceIndex, Damage, ShotFromDirection);
	}
}

void UReplicatedFoliageISM::ReceiveRadialDamage(const TArray<int32>& Instances, const TArray<float>& Damages,
	class AController* InstigatedBy, FVector Origin, float MaxRadius, const class UDamageType* DamageType,
	AActor* DamageCauser)
{
	const auto Subsystem = GetWorld()->GetSubsystem<UReplicatedFoliageSubsystem>();

	if (auto ReplicatedFoliageManager = Subsystem->GetOrSpawnReplicatedFoliageManager())
	{
		for (int32 i = 0; i < Instances.Num(); i++)
		{
			FTransform Transform;
			GetInstanceTransform(Instances[i], Transform, true);
			const FVector Direction = (Transform.GetLocation() - Origin).GetSafeNormal();
			
			ReplicatedFoliageManager->InstanceGetDamaged(this, Instances[i], Damages[i], Direction);
		}
	}
}

void UReplicatedFoliageISM::RestoreRemovedInstances()
{
	const auto Subsystem = GetWorld()->GetSubsystem<UReplicatedFoliageSubsystem>();
	auto ReplicatedFoliageManager = Subsystem->GetOrSpawnReplicatedFoliageManager();
	
	if (!ReplicatedFoliageManager)
	{
		return;
	}

	TArray<int32> InstancesToRemove;
	
	for (auto RepState : ReplicatedFoliageManager->ReplicatedArrayState.Items)
	{
		if (RepState.InstanceComponentGuid == IdentityId && RepState.InstanceFloat <= 0.0f)
		{
			InstancesToRemove.Add(RepState.InstanceIndex);
		}
	}

	if (InstancesToRemove.Num() > 0)
	{
		RemoveInstances(InstancesToRemove);
	}
}
