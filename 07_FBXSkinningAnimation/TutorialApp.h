#pragma once
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
class TutorialApp :
    public GameApp,public IImGuiRenderable,public IWorldEvent
{
public:
	TutorialApp(HINSTANCE hInstance);
	~TutorialApp();

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
	virtual void ImGuiRender();
	virtual void OnBeginPlay(World* pWorld);
	virtual void OnEndPlay(World* pWorld);
};

