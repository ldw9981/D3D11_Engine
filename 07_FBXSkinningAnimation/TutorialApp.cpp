#include "pch.h"
#include "TutorialApp.h"
#include "../Common/D3DRenderManager.h"
#include "../Common/SkeletalMeshComponent.h"
#include "../Common/SkeletalMeshActor.h"
#include "../Common/SkeletalMeshResource.h"
#include "../Common/StaticMeshComponent.h"
#include "../Common/StaticMeshActor.h"

#include "../Common/PlayerController.h"
#include "../Common/DefaultPawn.h"
#include "../Common/Pawn.h"



const Math::Vector3 START_POSITION = { 0.f, 0.f, -1000.f};

using namespace std;

TutorialApp::TutorialApp(HINSTANCE hInstance)
	:GameApp(hInstance),m_World("MyWorld")
{
	m_bUseConsole = true;
}

TutorialApp::~TutorialApp()
{
	
}

bool TutorialApp::Initialize(UINT Width, UINT Height)
{
	__super::Initialize(Width, Height);

	D3DRenderManager::Instance->SetImGuiRender(this);	

	
	IncreaseModel();
//	IncreaseModel();
//	IncreaseModel();
	

	m_pPlayerController = m_World.CreateGameObject<PlayerController>().get();
	m_pDefaultPawn = m_World.CreateGameObject<DefaultPawn>().get();
	m_pDefaultPawn->SetWorldPosition(Math::Vector3(0.0f,100.0f,-500.0f));
	m_pPlayerController->Posess(m_pDefaultPawn);
	m_World.SetWorldEvent(this);
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
	switch (message) 
	{	
	case WM_KEYDOWN:
		if (wParam == VK_UP) 
		{
			IncreaseModel();
			std::cout << "Test" << endl;
		}
		if (wParam == VK_DOWN)
		{
			DecreaseModel();
		}
		break;
	}
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

		int range = 1000;
		float posx = (float)(rand() % range) - range * 0.5f;
		float posy = (float)(rand() % range) - range * 0.5f;
		float posz = (float)(rand() % range) - range * 0.5f;
		SkActor->SetWorldPosition(Math::Vector3(posx, posy, posz));

		auto pRsc = pComponent->GetSceneResource();
		int playindex = rand() % pRsc-> m_Animations.size();
		pComponent->PlayAnimation(playindex);

		m_SpawnedActors.push_back(SkActor.get());
	}
	
	{
		auto StActor = m_World.CreateGameObject<StaticMeshActor>();
		StaticMeshComponent* pComponent = (StaticMeshComponent*)StActor->GetRootComponent();
		pComponent->ReadSceneResourceFromFBX("../Resource/angel_armor.FBX");
	

		int range = 1000;
		float posx = (float)(rand() % range) - range * 0.5f;
		float posy = (float)(rand() % range) - range * 0.5f;
		float posz = (float)(rand() % range) - range * 0.5f;
		StActor->SetWorldPosition(Math::Vector3(posx, posy, posz));
		
		m_SpawnedActors.push_back(StActor.get());
	}
	
	
}

void TutorialApp::DecreaseModel()
{
	auto it = m_SpawnedActors.begin();
	if (it == m_SpawnedActors.end())
		return;

	//(it)
	m_World.DestroyGameObject(*it);
	m_SpawnedActors.erase(it);
}

void TutorialApp::ImGuiRender()
{	
	ImGui::Text("%s",__FUNCTION__);
	ImGui::Text("다른곳에서도 ImGUI를 사용하는테스트 English Only");
}

void TutorialApp::OnBeginPlay(World* pWorld)
{
	LOG_MESSAGEA("%s World OnBeginPlay",pWorld->m_Name.c_str());
}

void TutorialApp::OnEndPlay(World* pWorld)
{
	LOG_MESSAGEA("%s World OnEndPlay", pWorld->m_Name.c_str());
}

