// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ReplicatedFoliageSettings.generated.h"

class AReplicatedFoliageManager;

/**
 * 
 */
UCLASS(config = ReplicatedFoliage, defaultconfig, notplaceable)
class REPLICATEDFOLIAGE_API UReplicatedFoliageSettings : public UObject
{
	GENERATED_BODY()

public:
	UReplicatedFoliageSettings(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(Config, EditAnywhere, meta=(MetaClass = "/Script/ReplicatedFoliage.ReplicatedFoliageManager"))
	FSoftClassPath GlobalReplicatedFoliageManagerClass;

	TSubclassOf<AReplicatedFoliageManager> GetReplicatedFoliageManagerClass() const;
};
