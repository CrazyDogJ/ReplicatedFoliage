// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ReplicatedFoliageSubsystem.generated.h"

class UReplicatedFoliageISM;
class AReplicatedFoliageManager;

USTRUCT(BlueprintType)
struct FFoliageInstanceRemoveContext
{
	GENERATED_BODY()

	/** Instance transform. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform Transform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* StaticMesh = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector ForceDirection = FVector::ZeroVector;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFoliageInstanceRemoved, FFoliageInstanceRemoveContext, OutContext);

/**
 * A useful system to get foliage global replication manager and ISM comps.
 * Instead of getting actor using world actor itr.
 */
UCLASS()
class REPLICATEDFOLIAGE_API UReplicatedFoliageSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnFoliageInstanceRemoved OnFoliageInstanceRemovedEvent;
	
	UFUNCTION(BlueprintPure)
	AReplicatedFoliageManager* GetReplicatedFoliageManager() const { return ReplicatedFoliageManager; }
	
	/** Used for client to set replicated foliage manager when manager begin play. */
	void SetReplicatedFoliageManager_Client(AReplicatedFoliageManager* Manager);
	
	/** Can call in server and clients, if no manager, server side will spawn one. */
	AReplicatedFoliageManager* GetOrSpawnReplicatedFoliageManager();

	/** Reg comp */
	void RegisterIsmComponent(UReplicatedFoliageISM* IsmComp);
	
	/** Unreg comp */
	void UnregisterIsmComponent(const UReplicatedFoliageISM* IsmComp);
	
	/** Get registered ism comps. */
	TSet<UReplicatedFoliageISM*> GetReplicatedFoliageIsmComps() const;
	
private:
	UPROPERTY()
	AReplicatedFoliageManager* ReplicatedFoliageManager;

	UPROPERTY()
	TSet<UReplicatedFoliageISM*> ReplicatedIsmComponents;
};
