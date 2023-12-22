#include "pch.h"
#include "TutorialApp.h"
#include "../Common/D3DRenderManager.h"
#include "../Common/SkeletalMeshComponent.h"
#include "../Common/StaticMeshComponent.h"
#include "../Common/SkeletalMeshResource.h"
#include "../Common/StaticMeshResource.h"
#include "../Common/ResourceManager.h"

TutorialApp::TutorialApp(HINSTANCE hInstance)
	:GameApp(hInstance)
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
	case WM_KEYDOWN:
		if (wParam == VK_UP) {
			IncreaseModel();
		}
		else if (wParam == VK_DOWN) {
			DecreaseModel();
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

		auto SceneResource = ResourceManager::Instance->CreateSkeletalMeshSceneResource("../Resource/Vampire.fbx");
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

		auto SceneResource = ResourceManager::Instance->CreateStaticMeshSceneResource("../Resource/box.fbx");
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
	
}

