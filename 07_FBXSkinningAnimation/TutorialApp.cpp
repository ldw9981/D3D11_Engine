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

	for (size_t i = 0; i < MAX_MODEL; i++)
	{
		m_Model[i].ReadFile(m_D3DRenderer->m_pDevice, "../Resource/SkinningTest.fbx");
		m_D3DRenderer->m_Models.push_back(&m_Model[i]);
	}

	
	return true;
}

void TutorialApp::Update()
{
	__super::Update();

	for (size_t i = 0; i < MAX_MODEL; i++)
	{
		m_Model[i].Update(GameTimer::m_Instance->DeltaTime());
	}
}

void TutorialApp::Render()
{
	m_D3DRenderer->Render();
}





