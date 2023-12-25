#include "pch.h"
#include "TutorialApp.h"
#include "../Common/D3DRenderManager.h"
#include "../Common/SkeletalMeshComponent.h"
#include "../Common/SkeletalMeshActor.h"
#include "../Common/StaticMeshComponent.h"
#include "../Common/StaticMeshActor.h"

#include "../Common/PlayerController.h"
#include "../Common/DefaultPawn.h"
#include "../Common/Pawn.h"


#include <utility>
#include <algorithm>


const Math::Vector3 START_POSITION = { 0.f, 0.f, -1000.f};

using namespace std;

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

	
	IncreaseModel();
	

	m_pPlayerController = m_World.CreateGameObject<PlayerController>();
	m_pDefaultPawn = m_World.CreateGameObject<DefaultPawn>();
	m_pDefaultPawn->SetWorldPosition(Math::Vector3(0.0f,100.0f,-500.0f));
	m_pPlayerController->Posess((Pawn*)m_pDefaultPawn.get());

	ChangeWorld(&m_World);

	return true;
}

void TutorialApp::Update()
{
	__super::Update();
}

void TutorialApp::Render()
{
	__super::Render();
}

LRESULT CALLBACK TutorialApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return GameApp::WndProc(hWnd, message, wParam, lParam);
}

void TutorialApp::IncreaseModel()
{	
	
	{
		auto SkActor = m_World.CreateGameObject<SkeletalMeshActor>();
		SkeletalMeshComponent* pComponent = (SkeletalMeshComponent*)SkActor->GetRootComponent();
		pComponent->ReadSceneResourceFromFBX("../Resource/Zombie.fbx");
		pComponent->AddSceneAnimationFromFBX("../Resource/Zombie_Run.fbx");
		pComponent->AddSceneAnimationFromFBX("../Resource/SkinningTest.fbx");

		int range = 500;
		float pos = (float)(rand() % range) - range * 0.5f;
		SkActor->SetWorldPosition(Math::Vector3(pos, 0.0f, 0.0f));

		//int playindex = rand() % model.GetSceneResource()->m_Animations.size();
		//model.PlayAnimation(playindex);
	}
	
	{
		auto StActor = m_World.CreateGameObject<StaticMeshActor>();
		StActor->m_SceneFilePath = "../Resource/ZeldaPosed001.fbx";

		int range = 500;
		float pos = (float)(rand() % range) - range * 0.5f;
		StActor->SetWorldPosition(Math::Vector3(pos, 0.0f, 0.0f));
		
	}
	
	
}

void TutorialApp::ImGuiRender()
{
	ImGui::Text("다른곳에서도 ImGUI를 사용하는테스트 English Only");
}

