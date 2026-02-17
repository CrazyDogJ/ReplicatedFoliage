// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicatedFoliageSettings.h"

#include "ReplicatedFoliageManager.h"

UReplicatedFoliageSettings::UReplicatedFoliageSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GlobalReplicatedFoliageManagerClass = FSoftClassPath(AReplicatedFoliageManager::StaticClass());
}

TSubclassOf<AReplicatedFoliageManager> UReplicatedFoliageSettings::GetReplicatedFoliageManagerClass() const
{
	return GlobalReplicatedFoliageManagerClass.TryLoadClass<AReplicatedFoliageManager>();
}
