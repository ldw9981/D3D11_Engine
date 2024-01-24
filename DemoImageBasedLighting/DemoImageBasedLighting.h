#pragma once
#include "resource.h"
#include "../Common/GameApp.h"

#include "../Common/Material.h"
#include "../Common/AnimationResource.h"
#include "../Common/Bone.h"

#include "../Common/World.h"
#include "../Common/StaticMeshComponent.h"
#include "../Common/SkeletalMeshComponent.h"
#include <functional>

using namespace DirectX::SimpleMath;
using namespace DirectX;

class PlayerController;
class DefaultPawn;
class EnvironmentActor;
class DemoApp :
	public GameApp
{
public:
	DemoApp(HINSTANCE hInstance);
	~DemoApp();

	World m_World;
	PlayerController* m_pPlayerController = nullptr;
	DefaultPawn* m_pDefaultPawn = nullptr;
	EnvironmentActor* m_pEnvironmentActor[3] = { nullptr,nullptr,nullptr};
	UINT m_Index= 0;
	UINT m_IndexPrev = 0;
	std::function<void()> m_ImGuiFunction;

	virtual bool Initialize(UINT Width, UINT Height);
	virtual void Update();
	virtual void Render();

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetupModel(int n, int distance); // 원점에서 부터 나선형으로 배치한다.

	virtual void OnBeginPlay(World* pWorld);
	virtual void OnEndPlay(World* pWorld);
};