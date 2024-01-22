// DemoShadowMap.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "DemoImageBasedLighting.h"

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

DemoApp::DemoApp(HINSTANCE hInstance)
	:GameApp(hInstance), m_World("MyWorld")
{
	m_bUseConsole = true;
}

DemoApp::~DemoApp()
{
	D3DRenderManager::Instance->RemoveImGuiRenderFunc(m_ImGuiFunction);
}

bool DemoApp::Initialize(UINT Width, UINT Height)
{
	__super::Initialize(Width, Height);
	

	std::string strCubeFBX = "../Resource/EnvironmentCube.FBX";
	std::wstring wstrEnvHDR[3] = { L"../Resource/RoomEnvHDR.dds", L"../Resource/BakerSampleEnvHDR.dds", L"../Resource/DaySkyEnvHDR.dds" };
	std::wstring wstrDiffuseHDR[3] = { L"../Resource/RoomDiffuseHDR.dds", L"../Resource/BakerSampleDiffuseHDR.dds", L"../Resource/DaySkyDiffuseHDR.dds" };
	std::wstring wstrSpecularHDR[3] = { L"../Resource/RoomSpecularHDR.dds", L"../Resource/BakerSampleSpecularHDR.dds", L"../Resource/DaySkySpecularHDR.dds" };
	std::wstring wstrBRDF[3] = { L"../Resource/RoomBRDF.dds", L"../Resource/BakerSampleBRDF.dds", L"../Resource/DaySkyBRDF.dds" };

	for(int i=0;i<3;++i)
	{
		m_pEnvironmentActor[i] = m_World.CreateGameObject<EnvironmentActor>().get();
		EnvironmentMeshComponent* pComponent = (EnvironmentMeshComponent*)m_pEnvironmentActor[i]->GetComponentPtrByName("EnvironmentMeshComponent");
		pComponent->ReadEnvironmentMeshFromFBX(strCubeFBX);
		pComponent->ReadEnvironmentTextureFromDDS(wstrEnvHDR[i]);
		pComponent->ReadIBLDiffuseTextureFromDDS(wstrDiffuseHDR[i]);
		pComponent->ReadIBLSpecularTextureFromDDS(wstrSpecularHDR[i]);
		pComponent->ReadIBLBRDFTextureFromDDS(wstrBRDF[i]);
		pComponent->SetLocalScale(Vector3(100.0f, 100.0f, 100.0f));
	}	

	D3DRenderManager::Instance->SetEnvironment(m_pEnvironmentActor[0]);
	D3DRenderManager::Instance->SetDirectionLight(Math::Vector3(0.0f,-0.9f,-0.1f));
	SetupModel(4, 500);

	m_pPlayerController = m_World.CreateGameObject<PlayerController>().get();
	m_pDefaultPawn = m_World.CreateGameObject<DefaultPawn>().get();
	m_pDefaultPawn->SetWorldPosition(Math::Vector3(0.0f, 130.0f, 200.0f));
	m_pPlayerController->Posess(m_pDefaultPawn);
	m_World.SetWorldEvent(this);
	ChangeWorld(&m_World);


	m_ImGuiFunction = [&]()
	{
		ImGui::Begin("IBL");
		ImGui::SetWindowPos(ImVec2(900,0));
		ImGui::Text("Environment");
		ImGui::RadioButton("Room", (int*)&m_Index, 0);
		ImGui::RadioButton("BakerSample", (int*)&m_Index, 1);
		ImGui::RadioButton("DaySky", (int*)&m_Index, 2);
		ImGui::End();
	};
	D3DRenderManager::Instance->AddImGuiRenderFunc(m_ImGuiFunction);

	return true;
}

void DemoApp::Update()
{
	__super::Update();
	if (m_IndexPrev != m_Index)
	{
		m_IndexPrev = m_Index;
		D3DRenderManager::Instance->SetEnvironment(m_pEnvironmentActor[m_Index]);
	}
}

void DemoApp::Render()
{
	__super::Render();
}

LRESULT CALLBACK DemoApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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


void DemoApp::SetupModel(int n, int distance)
{
	std::vector<std::string> staticMesh;	
	//staticMesh.push_back("../Resource/Torus.FBX");
	//staticMesh.push_back("../Resource/sphere.FBX");
	staticMesh.push_back("../Resource/ZeldaPosed001.FBX");
	

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
			StActor->SetWorldPosition(Math::Vector3((float)x * distance, 50.0f, (float)y * distance));
			BoxComponent* pCollisionComponent = (BoxComponent*)StActor->GetComponentPtrByName("BoxComponent");
			pCollisionComponent->SetCollisionType(CollisionType::NoCollision);		
		}
	}
}

void DemoApp::OnBeginPlay(World* pWorld)
{
	LOG_MESSAGEA("%s World OnBeginPlay", pWorld->m_Name.c_str());
}

void DemoApp::OnEndPlay(World* pWorld)
{
	LOG_MESSAGEA("%s World OnEndPlay", pWorld->m_Name.c_str());
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	DemoApp App(hInstance);  // 생성자에서 아이콘,윈도우 이름만 바꾼다
	if (!App.Initialize(1280, 960))
		return -1;

	return App.Run();
}
