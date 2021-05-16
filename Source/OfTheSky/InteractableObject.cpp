// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerInteractionComponent.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	staticMesh->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);

	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PropellerAudioComp"));
	audioComponent->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform);
	audioComponent->bAutoActivate = false;

	collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	collider->AttachToComponent(staticMesh, FAttachmentTransformRules::KeepRelativeTransform);
	collider->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableObject::Activate()
{
	if (isLocked) return;
	RenderOutline(true, false);
	OnInteraction();

	if (isActive) return;
	isActive = true;

	if (soundOnActivation != nullptr)
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
			audioComponent->SetSound(soundOnActivation);
			audioComponent->Play();
			}, soundActivationDelay, 1);
	}

	SetActorTickEnabled(true);
}

void AInteractableObject::Deactivate()
{
	if (isLocked) return;
	AfterInteraction();
	isActive = false;
	RenderOutline(false, false);
	SetActorTickEnabled(false);
}

void AInteractableObject::SetLocked(bool locked)
{
	isLocked = locked;
}

void AInteractableObject::RenderOutline(bool render, bool overrideLock)
{
	if (isLocked && !overrideLock) goto hideHUD;

	staticMesh->SetRenderCustomDepth(render);

	if (render && showInteractMessage)
	{
		printf("Show HUD");
		// show HUD
		return;
	}

hideHUD:
	printf("Hide HUD");
	// hide HUD
}

void AInteractableObject::OnInteraction_Implementation()
{
}

void AInteractableObject::AfterInteraction_Implementation()
{
}

