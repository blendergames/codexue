// Top-down player character with Enhanced Input movement

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TopDownCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(Blueprintable)
class TEST55CPP_API ATopDownCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ATopDownCharacter();

protected:
    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UCameraComponent> TopDownCamera;

    // Temporary visual until skeletal mesh is ready
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> BodyMesh;

    // Enhanced Input references (set in editor/blueprint; do not create in code)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> MoveAction;

    // If true, automatically add DefaultMappingContext on BeginPlay
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    bool bAddDefaultMappingContext = true;

    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    // Input handlers
    void HandleMove(const FInputActionValue& Value);
};
