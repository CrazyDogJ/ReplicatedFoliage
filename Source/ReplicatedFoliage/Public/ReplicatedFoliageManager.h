// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReplicatedFoliageFastArray.h"
#include "GameFramework/Actor.h"
#include "ReplicatedFoliageManager.generated.h"

class UReplicatedFoliageISM;

/**
 * An always relevant foliage replication manager actor.
 * Available on server and clients.
 */
UCLASS()
class REPLICATEDFOLIAGE_API AReplicatedFoliageManager : public AActor
{
	GENERATED_BODY()

public:
	AReplicatedFoliageManager();

	// Main replicated variables.
	UPROPERTY(Replicated, BlueprintReadOnly, SaveGame)
	FFoliageInstanceStateArray ReplicatedArrayState;

	// Call it when game is loaded and the ReplicatedArrayState is restored.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void PostGameLoaded();
	
	/** Only on clients start. !Exclude Listen Server! */
	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize);
	/** Only on clients end. !Exclude Listen Server! */

	/** Only on server. */
	void InstanceGetDamaged(UReplicatedFoliageISM* IsmPtr, int InstanceIndex, float Damage, FVector Direction);
	
	/** Check should remove instance at server and clients. */
	void CheckRemoveInstance(const FFoliageInstanceStateEntry& Item, FVector Direction = FVector::Zero());
	
	/** Remove instance at server and clients. */
	void RemoveInstance(const FGuid CompGuid, const int Index, FVector Direction);
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
