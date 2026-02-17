// Fill out your copyright notice in the Description page of Project Settings.

#include "ReplicatedFoliageFastArray.h"

#include "ReplicatedFoliageManager.h"

void FFoliageInstanceStateArray::PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize)
{
	if (Manager) Manager->PreReplicatedRemove(RemovedIndices, FinalSize);
}

void FFoliageInstanceStateArray::PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize)
{
	if (Manager) Manager->PostReplicatedAdd(AddedIndices, FinalSize);
}

void FFoliageInstanceStateArray::PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize)
{
	if (Manager) Manager->PostReplicatedChange(ChangedIndices, FinalSize);
}

bool FFoliageInstanceStateArray::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
{
	return FastArrayDeltaSerialize<FFoliageInstanceStateEntry, FFoliageInstanceStateArray>(Items, DeltaParms, *this);
}
