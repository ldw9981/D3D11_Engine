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
#include "../Common/BoxComponent.h"
#include "../Common/EnvironmentActor.h"
#include "../Common/EnvironmentMeshComponent.h"

const Math::Vector3 START_POSITION = { 0.f, 0.f, -1000.f};

using namespace std;

TutorialApp::TutorialApp(HINSTANCE hInstance)
	:GameApp(hInstance),m_World("MyWorld")
{
	m_bUseConsole = true;
	D3DRenderManager::Instance->AddImguiRenderable(this);
}

TutorialApp::~TutorialApp()
{

}

bool TutorialApp::Initialize(UINT Width, UINT Height)
{
	__super::Initialize(Width, Height);
	{
		m_pEnvironmentActor = m_World.CreateGameObject<EnvironmentActor>().get();
		EnvironmentMeshComponent* pComponent = (EnvironmentMeshComponent*)m_pEnvironmentActor->GetComponentPtrByName("EnvironmentMeshComponent");
		pComponent->ReadEnvironmentMeshFromFBX("../Resource/EnvironmentCube.fbx");
		pComponent->ReadEnvironmentTextureFromDDS(L"../Resource/RoomEnvHDR.dds");
		pComponent->ReadIBLDiffuseTextureFromDDS(L"../Resource/RoomDiffuseHDR.dds");
		pComponent->ReadIBLSpecularTextureFromDDS(L"../Resource/RoomSpecularHDR.dds");
		pComponent->ReadIBLBRDFTextureFromDDS(L"../Resource/RoomBRDF.dds");

		pComponent->SetLocalScale(Vector3(100.0f, 100.0f, 100.0f));
		
		auto wpComponent = m_pEnvironmentActor->GetComponentWeakPtrByName("EnvironmentMeshComponent");
		D3DRenderManager::Instance->SetEnvironment(std::dynamic_pointer_cast<EnvironmentMeshComponent>(wpComponent.lock()));
	}
	
	IncreaseSkeletalMeshModel();
	IncreaseStaticMeshModel();
	IncreaseStaticMeshModel();

	m_pPlayerController = m_World.CreateGameObject<PlayerController>().get();
	m_pDefaultPawn = m_World.CreateGameObject<DefaultPawn>().get();
	m_pDefaultPawn->SetWorldPosition(Math::Vector3(0.0f,0.0f,-500.0f));
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
			IncreaseSkeletalMeshModel();
			IncreaseStaticMeshModel();
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

void TutorialApp::IncreaseSkeletalMeshModel()
{	
	auto SkActor = m_World.CreateGameObject<SkeletalMeshActor>();
	SkeletalMeshComponent* pSkeletalMeshComponent = (SkeletalMeshComponent*)SkActor->GetComponentPtrByName("SkeletalMeshComponent");	
	pSkeletalMeshComponent->ReadSceneResourceFromFBX("../Resource/Zombie.fbx");
	pSkeletalMeshComponent->AddSceneAnimationFromFBX("../Resource/Zombie_Run.fbx");
	pSkeletalMeshComponent->AddSceneAnimationFromFBX("../Resource/SkinningTest.fbx");
	

	BoxComponent* pCollisionComponent = (BoxComponent*)SkActor->GetComponentPtrByName("BoxComponent");
	pCollisionComponent->SetLocalPosition(Vector3(0.0f, pSkeletalMeshComponent->m_BoundingBox.Extents.y, 0.0f));

	int range = 500;
	float posx = (float)(rand() % range) - range * 0.5f;	
	float posz = (float)(rand() % range) - range * 0.5f;
	SkActor->SetWorldPosition(Math::Vector3(posx, 0.0f, posz));

	auto pRsc = pSkeletalMeshComponent->GetSceneResource();
	int playindex = rand() % pRsc-> m_Animations.size();
	pSkeletalMeshComponent->PlayAnimation(playindex);

	m_SpawnedActors.push_back(SkActor.get());
}

void TutorialApp::IncreaseStaticMeshModel()
{
	std::vector<std::string> staticMesh;
	staticMesh.push_back("../Resource/char.FBX");
	staticMesh.push_back("../Resource/cerberus.FBX");
	staticMesh.push_back("../Resource/sphere.FBX");
	
	for (int i=0; i < staticMesh.size(); ++i)
	{
		auto StActor = m_World.CreateGameObject<StaticMeshActor>();
		StaticMeshComponent* pStaticMeshComponent = (StaticMeshComponent*)StActor->GetComponentPtrByName("StaticMeshComponent");
		pStaticMeshComponent->ReadSceneResourceFromFBX(staticMesh[i]);
		
		BoxComponent* pCollisionComponent = (BoxComponent*)StActor->GetComponentPtrByName("BoxComponent");
		//pCollisionComponent->SetLocalPosition(Vector3(0.0f, pStaticMeshComponent->m_BoundingBox.Extents.y, 0.0f));

		int range = 800;
		float posx = (float)(rand() % range) - range * 0.5f;
		float posz = (float)(rand() % range) - range * 0.5f;
		StActor->SetWorldPosition(Math::Vector3(posx, 0.0f, posz));
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
	ImGui::Begin("TutorialApp");
	ImGui::Text("%s",__FUNCTION__);
	ImGui::Text("다른곳에서도 ImGUI를 사용하는테스트 English Only");
	ImGui::End();
}

void TutorialApp::OnBeginPlay(World* pWorld)
{
	LOG_MESSAGEA("%s World OnBeginPlay",pWorld->m_Name.c_str());
}

void TutorialApp::OnEndPlay(World* pWorld)
{
	LOG_MESSAGEA("%s World OnEndPlay", pWorld->m_Name.c_str());
}

