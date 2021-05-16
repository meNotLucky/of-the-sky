// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "InteractableObject.h"
#include "PlayerInteractionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OFTHESKY_API UPlayerInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(Category = "Object Interaction", EditAnywhere, BlueprintReadWrite)
		float fieldOfView = 60.0f;

	UPROPERTY(Category = "Object Interaction", VisibleAnywhere, BlueprintReadOnly)
		UCapsuleComponent* capsuleCollider;

	UPROPERTY(Category = "Object Interaction", VisibleAnywhere, BlueprintReadOnly)
		TArray<AInteractableObject*> objectsInRange;

	UPROPERTY(Category = "Object Interaction", VisibleAnywhere, BlueprintReadOnly)
		AInteractableObject* currentObject;

	UPROPERTY(Category = "Object Interaction", VisibleAnywhere, BlueprintReadOnly)
		AInteractableObject* activeObject;

	/** called when something enters the sphere component */
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** called when something leaves the sphere component */
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Sets default values for this component's properties
	UPlayerInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void UpdateObjectInteraction();
	void UpdateCurrentObject();
	float CalculateViewAngle(AInteractableObject* object);

		
};
