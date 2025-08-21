// Top-down player character implementation

#include "TopDownCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InputActionValue.h"

ATopDownCharacter::ATopDownCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    // Capsule size is set by ACharacter default (42, 96). Keep defaults.

    // Spring arm for top-down camera
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 1200.0f; // Distance to character
    CameraBoom->bDoCollisionTest = false;  // Keep camera fixed for top-down
    CameraBoom->bUsePawnControlRotation = false;
    CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f)); // Slightly pitched view

    // Camera
    TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    TopDownCamera->bUsePawnControlRotation = false;

    // Temporary visual mesh (static cube expected to be set in BP)
    BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
    BodyMesh->SetupAttachment(GetCapsuleComponent());
    BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BodyMesh->SetGenerateOverlapEvents(false);
    BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -88.f)); // Sit on ground with default capsule

    // Character movement setup for top-down
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    UCharacterMovementComponent* MoveComp = GetCharacterMovement();
    MoveComp->bOrientRotationToMovement = true; // Rotate toward movement
    MoveComp->RotationRate = FRotator(0.f, 540.f, 0.f);
    MoveComp->bConstrainToPlane = true;
    MoveComp->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Z);
    MoveComp->bSnapToPlaneAtStart = true;
    MoveComp->MaxWalkSpeed = 600.f;
}

void ATopDownCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Add mapping context if provided externally (do not create in code)
    if (bAddDefaultMappingContext)
    {
        if (APlayerController* PC = Cast<APlayerController>(GetController()))
        {
            if (ULocalPlayer* LP = PC->GetLocalPlayer())
            {
                if (DefaultMappingContext)
                {
                    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
                    {
                        Subsystem->AddMappingContext(DefaultMappingContext, /*Priority*/0);
                    }
                }
            }
        }
    }
}

void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveAction)
        {
            EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::HandleMove);
            EIC->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATopDownCharacter::HandleMove);
            EIC->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ATopDownCharacter::HandleMove);
        }
    }
}

void ATopDownCharacter::HandleMove(const FInputActionValue& Value)
{
    const FVector2D MoveVec = Value.Get<FVector2D>();

    if (Controller == nullptr)
    {
        return;
    }

    // Use controller yaw to define movement plane directions
    const FRotator ControlRot = Controller->GetControlRotation();
    const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);

    const FVector ForwardDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
    const FVector RightDir   = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDir, MoveVec.Y);
    AddMovementInput(RightDir,   MoveVec.X);
}
