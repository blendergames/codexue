// Minimal GameMode for assigning a Pawn Blueprint in editor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TopDownGameMode.generated.h"

UCLASS()
class TEST55CPP_API ATopDownGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ATopDownGameMode();
};

