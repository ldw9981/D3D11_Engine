﻿// DemoShadowMap.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "DemoShadowMap.h"

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

DemoShadowMap::DemoShadowMap(HINSTANCE hInstance)
	:GameApp(hInstance), m_World("MyWorld")
{
	m_bUseConsole = true;	
}

DemoShadowMap::~DemoShadowMap()
{

}

bool DemoShadowMap::Initialize(UINT Width, UINT Height)
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

		auto wpComponent = m_pEnvironmentActor->GetComponentWeakPtrByName("EnvironmentMeshComponent");
		D3DRenderManager::Instance->SetEnvironment(std::dynamic_pointer_cast<EnvironmentMeshComponent>(wpComponent.lock()));
	}
	{
		auto StActor = m_World.CreateGameObject<StaticMeshActor>();
		StaticMeshComponent* pStaticMeshComponent = (StaticMeshComponent*)StActor->GetComponentPtrByName("StaticMeshComponent");
		pStaticMeshComponent->ReadSceneResourceFromFBX("../Resource/Ground.fbx");


		BoxComponent* pCollisionComponent = (BoxComponent*)StActor->GetComponentPtrByName("BoxComponent");


		StActor->SetWorldPosition(Math::Vector3(0.0f, 0.0f, 0.0f));

	}

	IncreaseSkeletalMeshModel();
	SetupModel(600, 500);

	m_pPlayerController = m_World.CreateGameObject<PlayerController>().get();
	m_pDefaultPawn = m_World.CreateGameObject<DefaultPawn>().get();
	m_pDefaultPawn->SetWorldPosition(Math::Vector3(0.0f, 200.0f, 0.0f));
	m_pPlayerController->Posess(m_pDefaultPawn);
	m_World.SetWorldEvent(this);
	ChangeWorld(&m_World);

	return true;
}

void DemoShadowMap::Update()
{
	__super::Update();
}

void DemoShadowMap::Render()
{
	__super::Render();
}

LRESULT CALLBACK DemoShadowMap::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

void DemoShadowMap::IncreaseSkeletalMeshModel()
{
	auto SkActor = m_World.CreateGameObject<SkeletalMeshActor>();
	SkeletalMeshComponent* pSkeletalMeshComponent = (SkeletalMeshComponent*)SkActor->GetComponentPtrByName("SkeletalMeshComponent");
	pSkeletalMeshComponent->ReadSceneResourceFromFBX("../Resource/Zombie.fbx");
	pSkeletalMeshComponent->AddSceneAnimationFromFBX("../Resource/Zombie_Run.fbx");
	pSkeletalMeshComponent->AddSceneAnimationFromFBX("../Resource/SkinningTest.fbx");

	BoxComponent* pCollisionComponent = (BoxComponent*)SkActor->GetComponentPtrByName("BoxComponent");
	pCollisionComponent->SetLocalPosition(Vector3(0.0f, pSkeletalMeshComponent->m_SceneResource->m_AABBmax.y * 0.5f, 0.0f));

	int range = 10000;
	float posx = (float)(rand() % range) - range * 0.5f;
	float posz = (float)(rand() % range) - range * 0.5f;
	SkActor->SetWorldPosition(Math::Vector3(posx, 30.0f, posz));

	auto pRsc = pSkeletalMeshComponent->GetSceneResource();
	int playindex = rand() % pRsc->m_Animations.size();
	pSkeletalMeshComponent->PlayAnimation(playindex);

	m_SpawnedActors.push_back(SkActor.get());
}

void DemoShadowMap::IncreaseStaticMeshModel()
{
	std::vector<std::string> staticMesh;
	staticMesh.push_back("../Resource/char.FBX");
	staticMesh.push_back("../Resource/cerberus.FBX");
	staticMesh.push_back("../Resource/sphere.FBX");

	for (int i = 0; i < staticMesh.size(); ++i)
	{
		auto StActor = m_World.CreateGameObject<StaticMeshActor>();
		StaticMeshComponent* pStaticMeshComponent = (StaticMeshComponent*)StActor->GetComponentPtrByName("StaticMeshComponent");
		pStaticMeshComponent->ReadSceneResourceFromFBX(staticMesh[i]);


		BoxComponent* pCollisionComponent = (BoxComponent*)StActor->GetComponentPtrByName("BoxComponent");
		pCollisionComponent->SetLocalPosition(Vector3(0.0f, pStaticMeshComponent->m_SceneResource->m_AABBmax.y * 0.5f, 0.0f));

		int range = 10000;
		float posx = (float)(rand() % range) - range * 0.5f;
		float posz = (float)(rand() % range) - range * 0.5f;
		StActor->SetWorldPosition(Math::Vector3(posx, 100.0f, posz));
		m_SpawnedActors.push_back(StActor.get());
	}
}

void DemoShadowMap::DecreaseModel()
{
	auto it = m_SpawnedActors.begin();
	if (it == m_SpawnedActors.end())
		return;

	//(it)
	m_World.DestroyGameObject(*it);
	m_SpawnedActors.erase(it);
}

void DemoShadowMap::SetupModel(int n, int distance)
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
			StActor->SetWorldPosition(Math::Vector3((float)x * distance, 50.0f, (float)y * distance));
			BoxComponent* pCollisionComponent = (BoxComponent*)StActor->GetComponentPtrByName("BoxComponent");
			pCollisionComponent->SetCollisionType(CollisionType::NoCollision);

			m_SpawnedActors.push_back(StActor.get());
		}
	}
}

void DemoShadowMap::OnBeginPlay(World* pWorld)
{
	LOG_MESSAGEA("%s World OnBeginPlay", pWorld->m_Name.c_str());
}

void DemoShadowMap::OnEndPlay(World* pWorld)
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


	DemoShadowMap App(hInstance);  // 생성자에서 아이콘,윈도우 이름만 바꾼다
	if (!App.Initialize(1280, 960))
		return -1;

	return App.Run();
}
