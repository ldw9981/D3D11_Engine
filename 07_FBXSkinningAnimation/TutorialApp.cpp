#include "pch.h"
#include "TutorialApp.h"
#include "../Common/D3DRenderManager.h"



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



	
	return true;
}

void TutorialApp::Update()
{
	__super::Update();

	for (auto& model : m_ModelList)
	{
		model.Update(m_Timer.DeltaTime());
	}


}

void TutorialApp::Render()
{
	for (auto& model : m_ModelList)
	{
		m_Renderer.AddModel(&model);
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
		else if (wParam == VK_LEFT) {
			DecreaseAnimationIndex();
		}
		else if (wParam == VK_RIGHT) {
			IncreaseAnimationIndex();
		}
		break;
	}

	return GameApp::WndProc(hWnd, message, wParam, lParam);
}

void TutorialApp::IncreaseModel()
{
	SkeletalMeshModel& model = m_ModelList.emplace_back();

	model.ReadSceneResourceFromFBX( "../Resource/Zombie.fbx");
	model.AddAnimationOnlyFromFBX("../Resource/Zombie_Run.fbx");
	model.AddAnimationOnlyFromFBX("../Resource/SkinningTest.fbx");
	model.PlayAnimation(0);
}

void TutorialApp::DecreaseModel()
{
	if (m_ModelList.empty())
		return;

	m_ModelList.pop_back();

}

void TutorialApp::IncreaseAnimationIndex()
{	
	for (auto& model : m_ModelList)
	{
		model.m_AnimationIndex++;
		if (model.m_AnimationIndex >= model.m_SceneResource->m_Animations.size())
			model.m_AnimationIndex = 0;
		model.PlayAnimation(model.m_AnimationIndex);
	}
}

void TutorialApp::DecreaseAnimationIndex()
{	
	for (auto& model : m_ModelList)
	{
		if (model.m_AnimationIndex == 0)
		{
			model.m_AnimationIndex = model.m_SceneResource->m_Animations.size() - 1;
		}
		else
		{
			model.m_AnimationIndex--;
		}	

		model.PlayAnimation(model.m_AnimationIndex);
	}
}
