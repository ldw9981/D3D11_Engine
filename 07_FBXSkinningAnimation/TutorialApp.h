#pragma once
#include "../Common/GameApp.h"

#include "../Common/Material.h"
#include "../Common/Animation.h"
#include "../Common/Bone.h"

#include "../Common/World.h"
#include "../Common/StaticMeshComponent.h"
#include "../Common/SkeletalMeshComponent.h"

#include <directXTK/Mouse.h>
#include <directXTK/Keyboard.h>

using namespace DirectX::SimpleMath;
using namespace DirectX;


class TutorialApp :
    public GameApp,public IImGuiRenderable
{
public:
	TutorialApp(HINSTANCE hInstance);
	~TutorialApp();


	World m_World;

	std::list<StaticMeshComponent>		m_StaticMeshModelList;
	std::list<SkeletalMeshModel>	m_SkeletalMeshModelList;


	std::unique_ptr<DirectX::Keyboard>              m_keyboard;
	std::unique_ptr<DirectX::Mouse>                 m_mouse;
	// FPS-style
	float                                           m_pitch;
	float                                           m_yaw;
	DirectX::SimpleMath::Vector3                    m_cameraPos;
	Vector3 m_forward, m_right;
	Matrix m_rotMatrix;
	Vector3 m_MoveDirection = { 0.0f, 0.0f, 0.0f };

	DirectX::Keyboard::KeyboardStateTracker         m_keys;
	DirectX::Mouse::ButtonStateTracker              m_mouseButtons;



	virtual bool Initialize(UINT Width, UINT Height);
	virtual void Update();
	virtual void Render();

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void IncreaseModel();
	void DecreaseModel();
	virtual void ImGuiRender();
};

