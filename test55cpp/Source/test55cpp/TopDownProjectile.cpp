// Simple forward-moving projectile implementation

#include "TopDownProjectile.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ATopDownProjectile::ATopDownProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->InitSphereRadius(10.f);
    Collision->SetCollisionProfileName(TEXT("Projectile"));
    Collision->SetNotifyRigidBodyCollision(true);
    Collision->OnComponentHit.AddDynamic(this, &ATopDownProjectile::OnHit);
    RootComponent = Collision;

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual"));
    VisualMesh->SetupAttachment(RootComponent);
    VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = Collision;
    ProjectileMovement->InitialSpeed = InitialSpeed;
    ProjectileMovement->MaxSpeed = MaxSpeed;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.f; // planar by default

    InitialLifeSpan = LifeSeconds;
}

void ATopDownProjectile::FireInDirection(const FVector& ShootDir)
{
    const FVector Dir = ShootDir.GetSafeNormal();
    ProjectileMovement->Velocity = Dir * InitialSpeed;
}

void ATopDownProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this && OtherActor != GetOwner())
    {
        Destroy();
    }
}
