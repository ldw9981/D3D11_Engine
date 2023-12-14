#include "pch.h"
#include "TutorialApp.h"




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

	m_Model.ReadFile(m_D3DRenderer.m_pDevice, "../Resource/SkinningTest.fbx");
	m_D3DRenderer.m_Models.push_back(&m_Model);
	return true;
}

void TutorialApp::Update()
{
	__super::Update();

	m_Model.Update(GameTimer::m_Instance->DeltaTime());	
}

void TutorialApp::Render()
{
	m_D3DRenderer.Render();
}





