#include "pch.h"
#include "TutorialApp.h"
#include "../Common/D3DRenderManager.h"
#include "../Common/SkeletalMeshComponent.h"
#include "../Common/StaticMeshComponent.h"
#include "../Common/SkeletalMeshResource.h"
#include "../Common/StaticMeshResource.h"
#include "../Common/ResourceManager.h"

#include <utility>
#include <algorithm>

constexpr float ROTATION_GAIN = 0.004f;
constexpr float MOVEMENT_GAIN = 0.07f;
const Math::Vector3 START_POSITION = { 0.f, 0.f, -1000.f};

using namespace std;

TutorialApp::TutorialApp(HINSTANCE hInstance)
	:GameApp(hInstance), m_cameraPos(START_POSITION)
{

}

TutorialApp::~TutorialApp()
{
	
}

bool TutorialApp::Initialize(UINT Width, UINT Height)
{
	__super::Initialize(Width, Height);

	D3DRenderManager::Instance->SetImGuiRender(this);

	Math::Vector3 RotationAngle;
	//Math::Quaternion rotQuaternion;
	//rotQuaternion = Math::Quaternion::CreateFromYawPitchRoll(RotationAngle.x, RotationAngle.y, RotationAngle.z);
	//Math::Matrix rotMatrix = Math::Matrix::CreateFromQuaternion(rotQuaternion);

	
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(m_hWnd);

	m_cameraPos = START_POSITION;
	m_yaw = m_pitch = 0.f;
	return true;
}

void TutorialApp::Update()
{
	__super::Update();
	for (auto& model : m_SkeletalMeshModelList)
	{
		model.Update(m_Timer.DeltaTime());
	}
	for (auto& model : m_StaticMeshModelList)
	{
		model.Update(m_Timer.DeltaTime());
	}

	auto mouse = m_mouse->GetState();
	m_mouseButtons.Update(mouse);
	if (mouse.positionMode == Mouse::MODE_RELATIVE)
	{
		Vector3 delta = Vector3(float(mouse.x), float(mouse.y), 0.f) * ROTATION_GAIN;
		m_pitch += delta.y;
		m_yaw += delta.x;
	}

	m_mouse->SetMode(mouse.rightButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);
	auto kb = m_keyboard->GetState();
	m_keys.Update(kb);
	if (kb.Home)
	{
		m_cameraPos = START_POSITION;
		m_yaw = m_pitch = 0.f;
	}

	Vector3 move = Vector3::Zero;
	float fowardScale = 0.0f;
	float rightScale = 0.0f;
	float upScale = 0.0f;
	if (kb.Up || kb.W)
		fowardScale = 1.f;

	if (kb.Down || kb.S)
		fowardScale = -1.f;

	if (kb.Left || kb.A)
		rightScale = -1.f;

	if (kb.Right || kb.D)
		rightScale = 1.f;

	if (kb.PageUp || kb.Space)
		upScale = 1.f;

	if (kb.PageDown || kb.X)
		upScale = -1.f;

	// 원래 있던 사인-코사인 어쩌고 함수는 전방벡터를 얻는 최적화된 코드
	
	m_rotMatrix = Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, 0.0f);
	m_forward = -m_rotMatrix.Forward();
	m_right = m_rotMatrix.Right();

	float speed = MOVEMENT_GAIN;       // 이동속도
	m_MoveDirection = m_forward * fowardScale + m_right * rightScale + Vector3::Up * upScale; // 회전상태와 키를 고려한 월드에서의 이동방향
	m_MoveDirection.Normalize(); //  순수 크기1로  정규화

	m_cameraPos = m_cameraPos + m_MoveDirection * speed;   // 정면벡터

	// limit pitch to straight up or straight down
	constexpr float limit = XM_PIDIV2 - 0.01f;
	m_pitch = max( -limit, m_pitch);
	m_pitch = min( +limit, m_pitch);

	// keep longitude in sane range by wrapping
	if (m_yaw > XM_PI)
	{
		m_yaw -= XM_2PI;
	}
	else if (m_yaw < -XM_PI)
	{
		m_yaw += XM_2PI;
	}

	XMVECTOR lookAt = m_cameraPos + m_forward;   // 정면벡터
	D3DRenderManager::Instance->m_CameraPos = m_cameraPos;
	D3DRenderManager::Instance->m_View = XMMatrixLookAtLH(m_cameraPos, lookAt, Vector3::Up);
}

void TutorialApp::Render()
{
	for (auto& model : m_SkeletalMeshModelList)
	{
		m_Renderer.AddMeshInstance(&model);
	}
	for (auto& model : m_StaticMeshModelList)
	{
		m_Renderer.AddMeshInstance(&model);
	}

	__super::Render();
}

LRESULT CALLBACK TutorialApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ACTIVATEAPP:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		break;
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		if (wParam == VK_UP) {
			IncreaseModel();
		}
		else if (wParam == VK_DOWN) {
			DecreaseModel();
		}
		break;

	case WM_SYSKEYDOWN:
		Keyboard::ProcessMessage(message, wParam, lParam);
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		{
			
		}
		break;	
	}

	return GameApp::WndProc(hWnd, message, wParam, lParam);
}

void TutorialApp::IncreaseModel()
{	
	/*
	{
		SkeletalMeshModel& model = m_SkeletalMeshModelList.emplace_back();

		auto SceneResource = ResourceManager::Instance->CreateSkeletalMeshSceneResource("../Resource/Zombie.fbx");
		SceneResource->AddAnimation(ResourceManager::Instance->CreateAnimation("../Resource/Zombie_Run.fbx"));
		SceneResource->AddAnimation(ResourceManager::Instance->CreateAnimation("../Resource/SkinningTest.fbx"));
		model.SetSceneResource(SceneResource);

		int range = 500;
		float pos = (float)(rand() % range) - range * 0.5f;
		model.m_Local = Matrix::CreateTranslation(pos, 0, 0);	
		int playindex = rand() % model.m_SceneResource->m_Animations.size();
		model.PlayAnimation(playindex);
	}
	*/
	
	
	{
		StaticMeshModel& model = m_StaticMeshModelList.emplace_back();

		auto SceneResource = ResourceManager::Instance->CreateStaticMeshSceneResource("../Resource/vampire.fbx");
		model.SetSceneResource(SceneResource);

		int range = 0;
//		float pos = (float)(rand() % range) - range * 0.5f;
		model.m_Local = Matrix::CreateTranslation(0, 0, 0);
		
	}
	
}

void TutorialApp::DecreaseModel()
{
	if (!m_SkeletalMeshModelList.empty())
	{
		m_SkeletalMeshModelList.pop_back();
	}
	
	if (!m_StaticMeshModelList.empty())
	{
		m_StaticMeshModelList.pop_back();
	}
	
}

void TutorialApp::ImGuiRender()
{
	ImGui::Text("다른곳에서도 ImGUI를 사용하는테스트 English Only");

	D3DRenderManager::Instance->AddDebugFloatToImGuiWindow("Control Yaw", m_yaw);
	D3DRenderManager::Instance->AddDebugFloatToImGuiWindow("Control Pitch", m_pitch);
	D3DRenderManager::Instance->AddDebugVector3ToImGuiWindow("CameraPos", m_cameraPos);
	
	D3DRenderManager::Instance->AddDebugMatrixToImGuiWindow("rotMatrix", m_rotMatrix);
	D3DRenderManager::Instance->AddDebugVector3ToImGuiWindow("Forward", m_forward);
	D3DRenderManager::Instance->AddDebugVector3ToImGuiWindow("Right", m_right);
	D3DRenderManager::Instance->AddDebugVector3ToImGuiWindow("MoveDirection", m_MoveDirection);
	

	


	
	D3DRenderManager::Instance->AddDebugMatrixToImGuiWindow("ViewMat",D3DRenderManager::Instance->m_View);
}

