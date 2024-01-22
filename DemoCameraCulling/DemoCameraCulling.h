#pragma once
#include "resource.h"
#include "../Common/GameApp.h"

#include "../Common/Material.h"
#include "../Common/AnimationResource.h"
#include "../Common/Bone.h"

#include "../Common/World.h"
#include "../Common/StaticMeshComponent.h"
#include "../Common/SkeletalMeshComponent.h"


using namespace DirectX::SimpleMath;
using namespace DirectX;

class PlayerController;
class DefaultPawn;
class EnvironmentActor;
class DemoCameraCulling :
	public GameApp, public IWorldEvent
{
public:
	DemoCameraCulling(HINSTANCE hInstance);
	~DemoCameraCulling();

	World m_World;
	std::list<Actor*> m_SpawnedActors;
	PlayerController* m_pPlayerController = nullptr;
	DefaultPawn* m_pDefaultPawn = nullptr;
	EnvironmentActor* m_pEnvironmentActor = nullptr;

	virtual bool Initialize(UINT Width, UINT Height);
	virtual void Update();
	virtual void Render();

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void IncreaseSkeletalMeshModel();
	void IncreaseStaticMeshModel();
	void DecreaseModel();
	void SetupModel(int n, int distance); // 원점에서 부터 나선형으로 배치한다.

	virtual void OnBeginPlay(World* pWorld);
	virtual void OnEndPlay(World* pWorld);
};