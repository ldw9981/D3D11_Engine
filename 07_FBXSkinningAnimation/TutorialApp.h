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
class TutorialApp :
    public GameApp,public IImGuiRenderable
{
public:
	TutorialApp(HINSTANCE hInstance);
	~TutorialApp();


	World m_World;

	std::list<StaticMeshComponent>		m_StaticMeshModelList;
	std::list<SkeletalMeshComponent>	m_SkeletalMeshModelList;

	std::shared_ptr<PlayerController> m_pPlayerController = nullptr;
	std::shared_ptr<DefaultPawn> m_pDefaultPawn = nullptr;

	virtual bool Initialize(UINT Width, UINT Height);
	virtual void Update();
	virtual void Render();

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void IncreaseModel();

	virtual void ImGuiRender();
};

