// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "InteractableObject.generated.h"

UCLASS()
class OFTHESKY_API AInteractableObject : public AActor
{
	GENERATED_BODY()

public:

	// note: components

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Object")
		USceneComponent* root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Object")
		UStaticMeshComponent* staticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Object")
		UBoxComponent* collider;

	// note: properties

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactable Object")
		bool isActive = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactable Object")
		bool isLocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Object")
		bool isToggle = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Object")
		bool activateOnCollision = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Object")
		bool showInteractMessage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Object")
		USoundCue* soundOnActivation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactable Object")
		UAudioComponent* audioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactable Object")
		float soundActivationDelay = 0;

	// note: functions

	UFUNCTION(BlueprintCallable, Category = "Interactable Object")
		void Activate();

	UFUNCTION(BlueprintCallable, Category = "Interactable Object")
		void Deactivate();

	UFUNCTION(BlueprintCallable, Category = "Interactable Object")
		void SetLocked(bool locked);

	UFUNCTION(BlueprintCallable, Category = "Interactable Object")
		void RenderOutline(bool render, bool overrideLock);

	AInteractableObject();

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable Object")
		void OnInteraction();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable Object")
		void AfterInteraction();

public:	

	virtual void Tick(float DeltaTime) override;

};
