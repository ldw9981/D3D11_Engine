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
	public GameApp
{
public:
	DemoCameraCulling(HINSTANCE hInstance);
	~DemoCameraCulling();

	World m_World;
	PlayerController* m_pPlayerController = nullptr;
	DefaultPawn* m_pDefaultPawn = nullptr;
	EnvironmentActor* m_pEnvironmentActor = nullptr;
	std::function<void(World*)> m_OnBeginPlayWorld = std::bind(&DemoCameraCulling::OnBeginPlayWorld, this, std::placeholders::_1);
	std::function<void(World*)> m_OnEndPlayWorld = std::bind(&DemoCameraCulling::OnEndPlayWorld, this, std::placeholders::_1);
public:
	virtual bool Initialize(UINT Width, UINT Height);
	virtual void Update();
	virtual void Render();

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetupModel(int n, int distance); // �������� ���� ���������� ��ġ�Ѵ�.

	void OnBeginPlayWorld(World* pWorld);
	void OnEndPlayWorld(World* pWorld);
};