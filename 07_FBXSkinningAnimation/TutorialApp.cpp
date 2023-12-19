#include "pch.h"
#include "TutorialApp.h"
#include "../Common/D3DRenderManager.h"
#include "../Common/SkeletalMeshComponent.h"
#include "../Common/StaticMeshComponent.h"
#include "../Common/SkeletalMeshResource.h"
#include "../Common/StaticMeshResource.h"


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
	{
		SkeletalMeshModel& model = m_SkeletalMeshModelList.emplace_back();

		int range = 500;
		float pos = (float)(rand() % range) - range * 0.5f;
		model.m_Local = Matrix::CreateTranslation(pos, 0, 0);

		model.ReadSceneResourceFromFBX("../Resource/Zombie.fbx");
		model.ReadAnimationOnlyFromFBX("../Resource/Zombie_Run.fbx");
		model.ReadAnimationOnlyFromFBX("../Resource/SkinningTest.fbx");

		int playindex = rand() % model.m_SceneResource->m_Animations.size();
		model.PlayAnimation(playindex);
	}
	

	{
		StaticMeshModel& model = m_StaticMeshModelList.emplace_back();

		int range = 500;
		float pos = (float)(rand() % range) - range * 0.5f;
		model.m_Local = Matrix::CreateTranslation(pos, 0, 0);
		model.ReadSceneResourceFromFBX("../Resource/ZeldaPosed001.fbx");	
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

}

