# UE5.5 탑다운 캐릭터/카메라/이동 구현 요약 (Enhanced Input)

프로젝트에 탑다운 카메라 구성과 보편적인 플레이어 캐릭터를 C++로 추가했습니다. 입력은 Enhanced Input을 사용하며, 입력 자산(IMC/IA)은 에디터에서 연결합니다.

## 구현 내용
- 캐릭터: `ATopDownCharacter` (`Source/test55cpp/TopDownCharacter.h`, `Source/test55cpp/TopDownCharacter.cpp`)
- 카메라: `USpringArmComponent` + `UCameraComponent`
  - 각도 기본 `Pitch -60°`, 카메라 충돌 비활성화, 컨트롤러 회전 미사용
- 임시 비주얼: `UStaticMeshComponent BodyMesh` 추가(충돌 없음) — 스켈레탈로 교체 예정
- 이동: Enhanced Input `Vector2D` 입력 기반 전/후(Y), 좌/우(X) 이동
- 이동 세팅: XY 평면 고정, 시작 시 평면 스냅, 이동 방향으로 Yaw 회전
- 코드에서 자산 생성 없음: IMC/IA는 에디터에서 할당
  - `UInputMappingContext* DefaultMappingContext`
  - `UInputAction* MoveAction`
  - `UInputAction* JumpAction`
  - `bool bAddDefaultMappingContext`(기본 true): `BeginPlay`에 IMC 추가
  - 점프: `Space` 등으로 `JumpAction` 트리거 → 평면 제약 해제 후 점프, 착지 시 재활성화

## 파일
- 추가: `Source/test55cpp/TopDownCharacter.h`, `Source/test55cpp/TopDownCharacter.cpp`
- 확인: `Source/test55cpp/test55cpp.Build.cs`에 `EnhancedInput` 의존성 포함
 - 추가: `Source/test55cpp/TopDownGameMode.h`, `Source/test55cpp/TopDownGameMode.cpp` (빈 GameMode)

## 에디터 설정 가이드
1) 블루프린트 생성
- `ATopDownCharacter` 기반 `BP_TopDownCharacter` 생성
- `BodyMesh`에 임시 `Cube`(BasicShapes) 할당, 필요하면 위치/스케일 조정
- `DefaultMappingContext`에 직접 만든 IMC 자산 할당
- `MoveAction`에 `Value Type = Axis2D`의 IA 자산 할당
- 다른 곳(예: PlayerController)에서 IMC를 추가한다면 `bAddDefaultMappingContext`를 꺼주세요

2) 월드 배치/기본 폰 설정
- 맵에 `BP_TopDownCharacter` 배치 후 플레이어가 Possess하도록 하거나
- GameMode에서 `Default Pawn Class`를 `BP_TopDownCharacter`로 지정

3) GameMode 사용(폰 BP 연결용)
- `ATopDownGameMode` 기반 블루프린트 `BP_TopDownGameMode` 생성
- `BP_TopDownGameMode`의 `Default Pawn Class`를 `BP_TopDownCharacter`로 설정
- 적용 방법 중 택1
  - 프로젝트 설정 > Maps & Modes > Default GameMode에 `BP_TopDownGameMode` 지정
  - 또는 레벨의 World Settings > GameMode Override에 `BP_TopDownGameMode` 지정

## 입력 자산 권장 예시
- `IA_Move` (Value: `Axis2D`)
  - 키보드: `W(0,+1)`, `S(0,-1)`, `A(-1,0)`, `D(+1,0)`
  - 게임패드: `Left Stick (2D Axis)`
- `IA_Jump` (Value: `Bool`)
  - 키보드: `Space Bar`
  - 트리거: `Started`에 Jump, `Completed/Canceled`에 StopJumping
- IMC: `IA_Move`를 위 키/축에 매핑
  - IMC에 `IA_Jump`도 추가

## 동작 요약
- 컨트롤러 Yaw 기준 앞/오른쪽 벡터를 계산하고 `AddMovementInput`에 적용
- 카메라는 스프링암에 고정되어 컨트롤러 회전에 영향 받지 않음
- 캐릭터는 이동 방향으로 자연스럽게 회전(`bOrientRotationToMovement = true`)
 - 점프 시 Z 평면 제약을 임시로 해제해 실제로 떠오르고, 착지하면 다시 XY 평면에 고정됨

## 참고
- 소스 위치: `Source/test55cpp/TopDownCharacter.*`
- 모듈 설정: `Source/test55cpp/test55cpp.Build.cs`
- UE 버전: 5.5 (타겟 설정 `IncludeOrderVersion = Unreal5_5` 확인)

## 다음 작업 제안(옵션)
- GameMode/PlayerController 클래스를 추가해 `Default Pawn` 지정 및 IMC 추가 위치를 명확히 분리
- 정사영(Orthographic) 탑다운이 필요하면 카메라 프로젝션 모드 옵션화
- 런/스프린트, 회피, 상호작용 등 입력 액션 확장
