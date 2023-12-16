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
		model.Update(GameTimer::m_Instance->DeltaTime());
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
		break;
	}

	return GameApp::WndProc(hWnd, message, wParam, lParam);
}

void TutorialApp::IncreaseModel()
{
	Model& model = m_ModelList.emplace_back();

	model.ReadFile(D3DRenderManager::m_pDevice, "../Resource/SkinningTest.fbx");
	
}

void TutorialApp::DecreaseModel()
{
	if (m_ModelList.empty())
		return;

	m_ModelList.pop_back();

}


