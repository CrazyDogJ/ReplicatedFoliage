// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "UObject/Object.h"
#include "ReplicatedFoliageFastArray.generated.h"

class AReplicatedFoliageManager;

USTRUCT(BlueprintType)
struct FFoliageInstanceStateEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, SaveGame)
	FGuid InstanceComponentGuid;

	UPROPERTY(BlueprintReadOnly, SaveGame)
	int InstanceIndex;
	
	UPROPERTY(BlueprintReadOnly, SaveGame)
	float InstanceFloat;

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool InstanceBool;
};

USTRUCT(BlueprintType)
struct FFoliageInstanceStateArray: public FFastArraySerializer
{
	GENERATED_BODY()

	FFoliageInstanceStateArray() {}
	FFoliageInstanceStateArray(AReplicatedFoliageManager* InManager) { Manager = InManager; }

	UPROPERTY(NotReplicated)
	AReplicatedFoliageManager* Manager = nullptr;
	
	UPROPERTY(BlueprintReadOnly, SaveGame)
	TArray<FFoliageInstanceStateEntry> Items;

	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize);
	
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);
};

template<>
struct TStructOpsTypeTraits<FFoliageInstanceStateArray> : public TStructOpsTypeTraitsBase2<FFoliageInstanceStateArray>
{
	enum { WithNetDeltaSerializer = true };
};
