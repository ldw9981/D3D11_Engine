#include "pch.h"
#include "framework.h"
#include "EditorApp.h"

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


const Math::Vector3 START_POSITION = { 0.f, 0.f, -1000.f };

using namespace std;

EditorApp::EditorApp(HINSTANCE hInstance)
	:GameApp(hInstance), m_World("MyWorld")
{	
	//*** 윈도우 클래스 정보 수정
	LoadStringW(hInstance, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_EDITORAPP, m_szWindowClass, MAX_LOADSTRING);	
	m_wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EDITORAPP));			
	m_wcex.lpszClassName = m_szWindowClass;
	m_wcex.hIconSm = LoadIcon(m_wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	

	m_bUseConsole = true;
	D3DRenderManager::Instance->m_OnRenderImGUI += m_OnRenderImGUI;
}

EditorApp::~EditorApp()
{
	D3DRenderManager::Instance->m_OnRenderImGUI -= m_OnRenderImGUI;
}

bool EditorApp::Initialize(UINT Width, UINT Height)
{
	__super::Initialize(Width, Height);
	{
		D3DRenderManager::Instance->m_bDebugShadow = true;
		m_pEnvironmentActor = m_World.CreateGameObject<EnvironmentActor>().get();
		EnvironmentMeshComponent* pComponent = (EnvironmentMeshComponent*)m_pEnvironmentActor->GetComponentPtrByName("EnvironmentMeshComponent");
		pComponent->ReadEnvironmentMeshFromFBX("../Resource/EnvironmentCube.fbx");
		pComponent->ReadEnvironmentTextureFromDDS(L"../Resource/RoomEnvHDR.dds");
		pComponent->ReadIBLDiffuseTextureFromDDS(L"../Resource/RoomDiffuseHDR.dds");
		pComponent->ReadIBLSpecularTextureFromDDS(L"../Resource/RoomSpecularHDR.dds");
		pComponent->ReadIBLBRDFTextureFromDDS(L"../Resource/RoomBRDF.dds");
		pComponent->SetLocalScale(Vector3(100.0f, 100.0f, 100.0f));
		D3DRenderManager::Instance->SetEnvironment(m_pEnvironmentActor);
	}
	{
		m_pStaticMeshActor = (StaticMeshActor*)m_World.CreateGameObject<StaticMeshActor>().get();
		StaticMeshComponent* pStaticMeshComponent = (StaticMeshComponent*)m_pStaticMeshActor->GetComponentPtrByName("StaticMeshComponent");
		pStaticMeshComponent->ReadSceneResourceFromFBX("../Resource/Ground.fbx");

		BoxComponent* pCollisionComponent = (BoxComponent*)m_pStaticMeshActor->GetComponentPtrByName("BoxComponent");
		m_pStaticMeshActor->SetWorldPosition(Math::Vector3(0.0f, 0.0f, 0.0f));
	}
	{
		auto SkActor = m_World.CreateGameObject<SkeletalMeshActor>();
		SkeletalMeshComponent* pSkeletalMeshComponent = (SkeletalMeshComponent*)SkActor->GetComponentPtrByName("SkeletalMeshComponent");
		pSkeletalMeshComponent->ReadSceneResourceFromFBX("../Resource/Zombie.fbx");
		pSkeletalMeshComponent->AddSceneAnimationFromFBX("../Resource/Zombie_Run.fbx");
		pSkeletalMeshComponent->AddSceneAnimationFromFBX("../Resource/SkinningTest.fbx");

		BoxComponent* pCollisionComponent = (BoxComponent*)SkActor->GetComponentPtrByName("BoxComponent");
		pCollisionComponent->SetLocalPosition(Vector3(0.0f, pSkeletalMeshComponent->m_SceneResource->m_AABBmax.y * 0.5f, 0.0f));

		SkActor->SetWorldPosition(Math::Vector3(0.0f, 0.0f, 800.0f));

		auto pRsc = pSkeletalMeshComponent->GetSceneResource();
		int playindex = rand() % pRsc->m_Animations.size();
		pSkeletalMeshComponent->PlayAnimation(playindex);
	}

	SetupModel(1000, 500);

	m_pPlayerController = m_World.CreateGameObject<PlayerController>().get();
	m_pDefaultPawn = m_World.CreateGameObject<DefaultPawn>().get();
	m_pDefaultPawn->SetWorldPosition(Math::Vector3(0.0f, 200.0f, 0.0f));
	m_pPlayerController->Posess(m_pDefaultPawn);

	ChangeWorld(&m_World);
	return true;
}

void EditorApp::Update()
{
	__super::Update();
	StaticMeshComponent* pStaticMeshComponent = (StaticMeshComponent*)m_pStaticMeshActor->GetComponentPtrByName("StaticMeshComponent");
	if (m_bShowGround)
	{
		pStaticMeshComponent->SetVisibility(true);
	}
	else
	{
		pStaticMeshComponent->SetVisibility(false);
	}

}

void EditorApp::Render()
{
	__super::Render();
}

LRESULT CALLBACK EditorApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_UP)
		{

		}
		if (wParam == VK_DOWN)
		{

		}
		break;
	}
	return GameApp::WndProc(hWnd, message, wParam, lParam);
}


void EditorApp::SetupModel(int n, int distance)
{
	std::vector<std::string> staticMesh;
	staticMesh.push_back("../Resource/Box.FBX");
	staticMesh.push_back("../Resource/Torus.FBX");
	staticMesh.push_back("../Resource/sphere.FBX");


	int x = 0, y = 0;

	// Initialize variables for the current direction and step size
	int dx = 0, dy = -1;

	// Loop to calculate coordinates in the spiral
	for (int i = 1; i <= n; ++i) {
		// Check if it's time to change direction
		if (x == y || (x < 0 && x == -y) || (x > 0 && x == 1 - y)) {
			// Change direction (rotate counterclockwise)
			int temp = dx;
			dx = -dy;
			dy = temp;
		}
		// Move to the next position
		x += dx;
		y += dy;

		{
			auto StActor = m_World.CreateGameObject<StaticMeshActor>();
			StaticMeshComponent* pStaticMeshComponent = (StaticMeshComponent*)StActor->GetComponentPtrByName("StaticMeshComponent");
			pStaticMeshComponent->ReadSceneResourceFromFBX(staticMesh[i % staticMesh.size()]);
			StActor->SetWorldPosition(Math::Vector3((float)x * distance, 40.0f, (float)y * distance));
			BoxComponent* pCollisionComponent = (BoxComponent*)StActor->GetComponentPtrByName("BoxComponent");
			pCollisionComponent->SetCollisionType(CollisionType::NoCollision);
		}
	}
}

void EditorApp::OnBeginPlayWorld(World* pWorld)
{
	LOG_MESSAGEA("%s World OnBeginPlayWorld", pWorld->m_Name.c_str());
}

void EditorApp::OnEndPlayWorld(World* pWorld)
{
	LOG_MESSAGEA("%s World OnEndPlayWorld", pWorld->m_Name.c_str());
}


void EditorApp::OnRenderImGUI()
{
	ImGui::Begin("DemoShadowMap");
	ImGui::Checkbox("ShowGround", &m_bShowGround);
	ImGui::End();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	EditorApp App(hInstance);  // 생성자에서 아이콘,윈도우 이름만 바꾼다
	if (!App.Initialize(1280, 960))
		return -1;

	return App.Run();
}