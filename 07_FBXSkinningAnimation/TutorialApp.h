#pragma once
#include "../Common/GameApp.h"
#include "../Common/World.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

class PlayerController;
class DefaultPawn;
class EnvironmentActor;

class TutorialApp :
    public GameApp
{
public:
	TutorialApp(HINSTANCE hInstance);
	~TutorialApp();

	World m_World;
	std::list<Actor*> m_SpawnedActors;
	PlayerController* m_pPlayerController = nullptr;
	DefaultPawn* m_pDefaultPawn = nullptr;
	EnvironmentActor* m_pEnvironmentActor = nullptr;

	std::function<void()> m_funcOnRenderImGUI;
	std::function<void(World*)> m_funcOnBeginPlayWorld;
	std::function<void(World*)> m_funcOnEndPlayWorld;

	virtual bool Initialize(UINT Width, UINT Height);
	virtual void Update();
	virtual void Render();

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void IncreaseSkeletalMeshModel();
	void IncreaseStaticMeshModel();
	void DecreaseModel();
	void SetupModel(int n, int distance);
	
	void OnRenderImGUI();
	void OnBeginPlayWorld(World* pWorld);
	void OnEndPlayWorld(World* pWorld);
};

