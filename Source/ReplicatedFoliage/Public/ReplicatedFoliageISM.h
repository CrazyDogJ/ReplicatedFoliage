// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Foliage/Public/FoliageInstancedStaticMeshComponent.h"
#include "ReplicatedFoliageISM.generated.h"

class AReplicatedFoliageManager;
/**
 * 
 */
UCLASS()
class REPLICATEDFOLIAGE_API UReplicatedFoliageISM : public UFoliageInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:
	UReplicatedFoliageISM();

	/** Used for replication identity. */
	UPROPERTY(VisibleAnywhere)
	FGuid IdentityId;

	/** Also used for save game loaded. */
	UFUNCTION(BlueprintCallable)
	void RestoreRemovedInstances();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	virtual void ReceivePointDamage(int32 InstanceIndex, float Damage, class AController* InstigatedBy, FVector HitLocation, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION()
	virtual void ReceiveRadialDamage(const TArray<int32>& Instances, const TArray<float>& Damages, class AController* InstigatedBy, FVector Origin, float MaxRadius, const class UDamageType* DamageType, AActor* DamageCauser);
};
