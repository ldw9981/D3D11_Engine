#include "pch.h"
#include "TutorialApp.h"
#include "../Common/D3DRenderManager.h"
#include "../Common/SkeletalMeshComponent.h"
#include "../Common/StaticMeshComponent.h"
#include "../Common/SkeletalMeshResource.h"
#include "../Common/StaticMeshResource.h"
#include "../Common/ResourceManager.h"
#include "../Common/PlayerController.h"

#include "../Common/DefaultPawn.h"
#include "../Common/Pawn.h"
#include "../Common/SkeletalMeshActor.h"

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
		SkeletalMeshComponent* SkComponent = (SkeletalMeshComponent*)SkActor->GetRootComponent().get();
		SkComponent->SetSceneFilePath("../Resource/Zombie.fbx");
		//SkComponent->AddSceneAnimationFromFBX("../Resource/Zombie_Run.fbx");
		//SkComponent->AddSceneAnimationFromFBX("../Resource/SkinningTest.fbx");

		int range = 500;
		float pos = (float)(rand() % range) - range * 0.5f;
		SkActor->SetWorldPosition(Math::Vector3(pos, 0.0f, 0.0f));

		//int playindex = rand() % model.GetSceneResource()->m_Animations.size();
		//model.PlayAnimation(playindex);
	}
	/*
	{
		StaticMeshComponent& model = m_StaticMeshModelList.emplace_back();

		auto SceneResource = ResourceManager::Instance->CreateStaticMeshSceneResource("../Resource/Vampire.fbx"); //cerberus
		model.SetSceneResource(SceneResource);
	}
	*/
	
}

void TutorialApp::ImGuiRender()
{
	ImGui::Text("다른곳에서도 ImGUI를 사용하는테스트 English Only");
}

