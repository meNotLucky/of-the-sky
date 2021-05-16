// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/InputSettings.h"

// Sets default values for this component's properties
UPlayerInteractionComponent::UPlayerInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPlayerInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	capsuleCollider = Cast<UCapsuleComponent>(GetOwner()->GetRootComponent());
	capsuleCollider->OnComponentBeginOverlap.AddDynamic(this, &UPlayerInteractionComponent::OnOverlapBegin);
	capsuleCollider->OnComponentEndOverlap.AddDynamic(this, &UPlayerInteractionComponent::OnOverlapEnd);
	if(capsuleCollider)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Collider set!"));

}


// Called every frame
void UPlayerInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateObjectInteraction();
	UpdateCurrentObject();
}

void UPlayerInteractionComponent::UpdateObjectInteraction()
{
	if (activeObject != nullptr)
		return;

	if (objectsInRange.Num() == 0)
	{
		if (currentObject != nullptr)
			currentObject->RenderOutline(false, false);
		currentObject = nullptr;
	}
	else
	{
		for (auto object : objectsInRange)
		{
			if (CalculateViewAngle(object) < fieldOfView)
			{
				if (currentObject == nullptr || currentObject == object)
				{
					currentObject = object;
					currentObject->RenderOutline(true, false);
				}
				else if (CalculateViewAngle(currentObject) > CalculateViewAngle(object))
				{
					currentObject->RenderOutline(false, false);
				}

				return;
			}

			object->RenderOutline(false, false);
			currentObject = nullptr;
		}
	}
}

void UPlayerInteractionComponent::UpdateCurrentObject()
{
	auto inputSettings = UInputSettings::GetInputSettings();
	TArray<FInputActionKeyMapping> mappings;
	inputSettings->GetActionMappingByName(FName("Interact"), mappings);

	if (activeObject == nullptr)
	{
		if (currentObject == nullptr)
			return;

		for (auto mapping : mappings)
		{
			auto controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (!controller->WasInputKeyJustPressed(mapping.Key))
				return;

			activeObject = currentObject;
			activeObject->Activate();
		}
	}
	else if (!activeObject->isLocked && activeObject->isActive)
	{
		for (auto mapping : mappings)
		{
			auto controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (!controller->WasInputKeyJustReleased(mapping.Key))
				return;

			activeObject->Deactivate();
		}
	}
	else
	{
		activeObject->RenderOutline(false, false);
		activeObject = nullptr;
	}
}

float UPlayerInteractionComponent::CalculateViewAngle(AInteractableObject* object)
{
	auto meshLocation = object->staticMesh->GetComponentLocation();
	auto actorTransform = GetOwner()->GetActorTransform();
	auto inversedLocation = UKismetMathLibrary::InverseTransformLocation(actorTransform, meshLocation);
	auto rotator = UKismetMathLibrary::Conv_VectorToRotator(inversedLocation);
	return abs(rotator.Yaw);
}

void UPlayerInteractionComponent::OnOverlapBegin(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	auto object = Cast<AInteractableObject>(OtherActor);
	if (!object) return;

	if (object->isToggle && !object->isActive)
	{
		if (object->activateOnCollision)
			object->Deactivate();
		else
			objectsInRange.Remove(object);

		return;
	}

	if (object->activateOnCollision)
		object->Activate();
	else
		objectsInRange.Add(object);

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::FromInt(objectsInRange.Num()));
}

void UPlayerInteractionComponent::OnOverlapEnd(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto object = Cast<AInteractableObject>(OtherActor);
	if (!object) return;

	if (object->isToggle)
		return;

	if (object->activateOnCollision)
		object->Deactivate();
	else
		objectsInRange.Remove(object);

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::FromInt(objectsInRange.Num()));
}

