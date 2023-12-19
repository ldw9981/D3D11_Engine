#pragma once
#include "../Common/GameApp.h"
#include "../Common/Mesh.h"
#include "../Common/Material.h"
#include "../Common/Animation.h"
#include "../Common/Node.h"
#include "../Common/Model.h"
#include "../Common/World.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;


class TutorialApp :
    public GameApp,public IImGuiRenderable
{
public:
	TutorialApp(HINSTANCE hInstance);
	~TutorialApp();


	World m_World;

	std::list<StaticMeshModel>		m_StaticMeshModelList;
	std::list<SkeletalMeshModel>	m_SkeletalMeshModelList;


	virtual bool Initialize(UINT Width, UINT Height);
	virtual void Update();
	virtual void Render();

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void IncreaseModel();
	void DecreaseModel();
	virtual void ImGuiRender();
};

