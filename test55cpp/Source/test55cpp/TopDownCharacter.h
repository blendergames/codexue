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
class ATopDownProjectile;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> FireAction;

    // If true, automatically add DefaultMappingContext on BeginPlay
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    bool bAddDefaultMappingContext = true;

    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    // Input handlers
    void HandleMove(const FInputActionValue& Value);

    // Jump handlers (Enhanced Input)
    void StartJump(const FInputActionValue& Value);
    void StopJump(const FInputActionValue& Value);

    void HandleFire(const FInputActionValue& Value);

private:
    // Projectile config
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    TSubclassOf<ATopDownProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float MuzzleForwardOffset = 60.f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float MuzzleUpOffset = 40.f;
};
