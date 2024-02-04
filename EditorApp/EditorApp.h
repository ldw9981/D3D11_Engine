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
class StaticMeshActor;
class EditorApp :
	public GameApp
{
public:
	EditorApp(HINSTANCE hInstance);
	~EditorApp();

	World m_World;

	PlayerController* m_pPlayerController = nullptr;
	DefaultPawn* m_pDefaultPawn = nullptr;
	EnvironmentActor* m_pEnvironmentActor = nullptr;
	StaticMeshActor* m_pStaticMeshActor = nullptr;

	std::function<void()> m_OnRenderImGUI = std::bind(&EditorApp::OnRenderImGUI, this);
	std::function<void(World*)> m_OnBeginPlayWorld = std::bind(&EditorApp::OnBeginPlayWorld, this, std::placeholders::_1);
	std::function<void(World*)> m_OnEndPlayWorld = std::bind(&EditorApp::OnEndPlayWorld, this, std::placeholders::_1);

	bool m_bShowGround = true;

	virtual bool Initialize(UINT Width, UINT Height);
	virtual void Update();
	virtual void Render();

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetupModel(int n, int distance); // 원점에서 부터 나선형으로 배치한다.

	void OnBeginPlayWorld(World* pWorld);
	void OnEndPlayWorld(World* pWorld);
	void OnRenderImGUI();
};