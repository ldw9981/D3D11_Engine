#include "pch.h"
#include "D3DRenderManager.h"

#include "Helper.h"
#include <d3dcompiler.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <winerror.h>
#include <dxgidebug.h>
#include <Psapi.h>

#include "SkeletalMeshComponent.h"
#include "StaticMeshComponent.h"
#include "AnimationResource.h"
#include "Material.h"	
#include "StaticMeshInstance.h"
#include "StaticMeshResource.h"

#include "SkeletalMeshInstance.h"
#include "SkeletalMeshResource.h"

#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"
#include "EnvironmentMeshComponent.h"
#include "CollisionComponent.h"
#include "SphereComponent.h"
#include "BoxComponent.h"
#include "CollisionManager.h"
#include "DebugDraw.h"
#include <DirectXTex.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")


#define SHADOWMAP_SIZE 2048


D3DRenderManager* D3DRenderManager::Instance = nullptr;



ID3D11Device* D3DRenderManager::m_pDevice = nullptr;

D3DRenderManager::D3DRenderManager()
 :m_BaseViewport {}
{
	assert(Instance == nullptr);
	Instance = this;
}

D3DRenderManager::~D3DRenderManager()
{
	Uninitialize();
}




void D3DRenderManager::AddMeshInstance(SkeletalMeshComponent* pModel)
{	
	for (size_t i = 0; i < pModel->m_MeshInstances.size(); i++)
	{
		auto* pMeshInstance = &pModel->m_MeshInstances[i];

		if (pMeshInstance->m_pMaterial->m_MaterialMapFlags & MaterialMapFlags::OPACITY)
		{
			m_SkeletalMeshInstanceTranslucent.push_back(pMeshInstance);
		}
		else
		{
			m_SkeletalMeshInstanceOpaque.push_back(pMeshInstance);
		}
	}
}

void D3DRenderManager::AddMeshInstance(StaticMeshComponent* pModel)
{
	for (size_t i = 0; i < pModel->m_MeshInstances.size(); i++)
	{
		auto* pMeshInstance = &pModel->m_MeshInstances[i];

		if (pMeshInstance->m_pMaterial->m_MaterialMapFlags & MaterialMapFlags::OPACITY)
		{
			m_StaticMeshInstanceTranslucent.push_back(pMeshInstance);
		}
		else
		{
			m_StaticMeshInstanceOpaque.push_back(pMeshInstance);
		}
	}
}

void D3DRenderManager::SortMeshInstance()
{

	//파이프라인에 설정하는 머터리얼의 텍스쳐 변경을 최소화 하기위해 머터리얼 별로 정렬한다.
	m_SkeletalMeshInstanceOpaque.sort([](const SkeletalMeshInstance* lhs, const SkeletalMeshInstance* rhs)
		{
			return lhs->m_pMaterial < rhs->m_pMaterial;
		});
	//파이프라인에 설정하는 머터리얼의 텍스쳐 변경을 최소화 하기위해 머터리얼 별로 정렬한다.
	m_StaticMeshInstanceOpaque.sort([](const StaticMeshInstance* lhs, const StaticMeshInstance* rhs)
		{
			return lhs->m_pMaterial < rhs->m_pMaterial;
		});

	//파이프라인에 설정하는 머터리얼의 텍스쳐 변경을 최소화 하기위해 머터리얼 별로 정렬한다.
	m_SkeletalMeshInstanceTranslucent.sort([](const SkeletalMeshInstance* lhs, const SkeletalMeshInstance* rhs)
		{
			return lhs->m_pMaterial < rhs->m_pMaterial;
		});

	//파이프라인에 설정하는 머터리얼의 텍스쳐 변경을 최소화 하기위해 머터리얼 별로 정렬한다.
	m_StaticMeshInstanceTranslucent.sort([](const StaticMeshInstance* lhs, const StaticMeshInstance* rhs)
		{
			return lhs->m_pMaterial < rhs->m_pMaterial;
		});
}

void D3DRenderManager::ClearMeshInstance()
{
	m_StaticMeshInstanceTranslucent.clear();
	m_SkeletalMeshInstanceTranslucent.clear();
	m_StaticMeshInstanceOpaque.clear();
	m_SkeletalMeshInstanceOpaque.clear();
}

bool D3DRenderManager::Initialize(HWND Handle,UINT Width, UINT Height)
{
	m_hWnd = Handle;
	HRESULT hr = 0;	// 결과값.

	// Create DXGI factory
	HR_T(CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)m_pDXGIFactory.GetAddressOf()));
	HR_T(m_pDXGIFactory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(m_pDXGIAdapter.GetAddressOf())));

	// 스왑체인 속성 설정 구조체 생성.
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = Handle;	// 스왑체인 출력할 창 핸들 값.
	swapDesc.Windowed = true;		// 창 모드 여부 설정.
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// 백버퍼(텍스처)의 가로/세로 크기 설정.
	swapDesc.BufferDesc.Width = Width;
	swapDesc.BufferDesc.Height = Height;
	// 화면 주사율 설정.
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;

	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	

	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// 1. 장치 생성.   2.스왑체인 생성. 3.장치 컨텍스트 생성.

	HR_T(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, 0,
		D3D11_SDK_VERSION, &swapDesc, m_pSwapChain.GetAddressOf(), &m_pDevice, NULL, m_pDeviceContext.GetAddressOf()));		
			
	//UINT samplecount, quality;
	//MSAACheck(DXGI_FORMAT_R8G8B8A8_UNORM, samplecount, quality);
	//assert(m_SampleQuality > quality && m_SampleCount > samplecount);

	SetBaseViewPort(Width,Height);
	SetShadowViewPort(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	CreateBuffers();
	
	
	/*
		ImGui 초기화.
	*/
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pDeviceContext.Get());

	// * Render() 에서 파이프라인에 바인딩할 버텍스 셰이더 생성
	CreateShaders();
	CreateEnvironment();
	CreateConstantBuffer();
	CreateSamplerState();
	CreateRasterizerState();
	CreateBlendState();
	
	// 화면 크기가 바뀌면 다시계산해야함
	m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_BaseViewport.Width / (FLOAT)m_BaseViewport.Height, 1.0f, 100000.0f);
	BoundingFrustum::CreateFromMatrix(m_Frustum, m_Projection);

	
	DebugDraw::Initialize(m_pDevice, m_pDeviceContext);
	return true;
}

void D3DRenderManager::Uninitialize()
{
	DebugDraw::Uninitialize();
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

#ifdef DEBUG_D3D11_LIVEDEVICE
	ComPtr<ID3D11Debug> pD3D11Dbug;
	HRESULT hr;
	hr = m_pDevice->QueryInterface(IID_PPV_ARGS(pD3D11Dbug.GetAddressOf()));
	pD3D11Dbug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);	
#endif // DEBUG_D3D11_LIVEDEVICE	
	SAFE_RELEASE(m_pDevice);
}



void D3DRenderManager::Update(float DeltaTime)
{	
	// 디렉션라이트방향
	m_ShadowProjection = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_ShadowViewport.Width / (FLOAT)m_ShadowViewport.Height, m_ShadowProjectionNearFar.x, m_ShadowProjectionNearFar.y);

	if (m_pCamera.expired() == false)
	{
		auto pCamera = m_pCamera.lock();
		Math::Vector3 eye = pCamera->m_World.Translation();
		m_LookAt = pCamera->m_World.Translation() + -pCamera->m_World.Forward();
		Math::Vector3 up = pCamera->m_World.Up();
		m_View = XMMatrixLookAtLH(eye, m_LookAt, up);
		m_Light.EyePosition = eye;	// HLSL 상수버퍼 갱신을 위한 데이터 업데이트
		if (m_bWorkCulling && !m_bFreezeCulling )  // 디버깅을 위해 culling 위치를  멈출수있음.
		{			
			BoundingFrustum::CreateFromMatrix(m_Frustum, m_Projection, false);
			m_Frustum.Transform(m_Frustum, pCamera->m_World);			
		}	

		float distForward = 1;
		float distFromLookAt = 3000;
		m_ShadowLootAt = pCamera->m_World.Translation();
		m_ShadowPos = m_ShadowLootAt +  (m_Light.Direction * -distFromLookAt);
		m_ShadowDir = m_ShadowLootAt - m_ShadowPos;
		m_ShadowDir.Normalize();
		m_ShadowView = XMMatrixLookAtLH(m_ShadowPos, m_ShadowLootAt, Vector3(0.0f,1.0f,0.0f));
	}
	m_Light.Direction.Normalize();
	m_pDeviceContext->UpdateSubresource(m_pCBDirectionLight.Get(), 0, nullptr, &m_Light, 0, 0);

	// 뷰프로젝션 업데이트
	m_TransformVP.mView = m_View.Transpose();
	m_TransformVP.mProjection = m_Projection.Transpose();
	m_TransformVP.mShadowView = m_ShadowView.Transpose();
	m_TransformVP.mShadowProjection = m_ShadowProjection.Transpose();
	m_pDeviceContext->UpdateSubresource(m_pCBTransformVP.Get(), 0, nullptr, &m_TransformVP, 0, 0);

	m_pDeviceContext->UpdateSubresource(m_pCBGlobal.Get(), 0, nullptr, &m_Global, 0, 0);
	m_cbMaterialOverride.SetData(m_pDeviceContext.Get(), m_MaterialOverride);

	m_nDrawComponentCount = 0;
	for (auto& SkeletalMeshComponent : m_SkeletalMeshComponents)
	{
		SkeletalMeshComponent->m_bIsCulled = false;
		if (!m_bWorkCulling || m_Frustum.Intersects(SkeletalMeshComponent->m_BoundingBox))
		{
			SkeletalMeshComponent->m_bIsCulled = true;
			AddMeshInstance(SkeletalMeshComponent);	// 하나의 메시 컴포넌트에 여러개의 메시 Instance 가 있을수있음.
			m_nDrawComponentCount++;
		}
	}

	for (auto& StaticMeshComponent : m_StaticMeshComponents)
	{
		StaticMeshComponent->m_bIsCulled = false;
		if (!m_bWorkCulling || m_Frustum.Intersects(StaticMeshComponent->m_BoundingBox))
		{
			StaticMeshComponent->m_bIsCulled = true;
			AddMeshInstance(StaticMeshComponent);  // 하나의 메시 컴포넌트에 여러개의 메시 Instance 가 있을수있음.
			m_nDrawComponentCount++;
		}		
	}

	SortMeshInstance();

	for (std::list<DebugRay>::iterator it = m_DebugDrawLines.begin(); it != m_DebugDrawLines.end() ; )
	{
		DebugRay& ray = *it;
		ray.time -= DeltaTime;
		if (ray.time <= 0)
			it = m_DebugDrawLines.erase(it);		
		else
			it++;
	}
}

//매 프레임 호출해야하는 것만 여기에 넣는다.  한번만 호출해도 되는것은 Initialize()에 넣는다.
void D3DRenderManager::Render()
{	
	// Clear the back buffer
	m_pDeviceContext->RSSetViewports(1,&m_ShadowViewport);
	const float clear_shadow[4] = { 0.0f ,  0.0f ,  0.0f, 1.0f };	
	m_pDeviceContext->OMSetRenderTargets(0, NULL, m_pShadowMapDSV.Get());
	m_pDeviceContext->ClearDepthStencilView(m_pShadowMapDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);	

	m_ShaderShadowSkeletalMesh.SetShader(m_pDeviceContext.Get());
	RenderSkeletalMeshInstanceOpaque();
	RenderSkeletalMeshInstanceTranslucent();
		
	m_ShaderShadowStaticMesh.SetShader(m_pDeviceContext.Get());
	RenderStaticMeshInstanceOpaque();
	RenderStaticMeshInstanceTranslucent();

	m_pDeviceContext->RSSetViewports(1, &m_BaseViewport);
	// Clear the back buffer
	const float clear_color_with_alpha[4] = { m_ClearColor.x , m_ClearColor.y , m_ClearColor.z, 1.0f };	
	m_pDeviceContext->ClearRenderTargetView(m_pBackBufferRTV.Get(), clear_color_with_alpha);
	m_pDeviceContext->ClearDepthStencilView(m_pDefaultDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pDeviceContext->OMSetRenderTargets(1, m_pBackBufferRTV.GetAddressOf(), m_pDefaultDSV.Get());
	m_pDeviceContext->PSSetShaderResources(11, 1, m_pShadowMapSRV.GetAddressOf()); // 한번에 7개의 텍스처를 설정한다.

	m_ShaderBaseSkeletalMesh.SetShader(m_pDeviceContext.Get());
	RenderSkeletalMeshInstanceOpaque();// 불투명 먼저

	m_ShaderBaseStaticMesh.SetShader(m_pDeviceContext.Get());
	RenderStaticMeshInstanceOpaque();				
	
	if (m_pEnvironmentMeshComponent.expired() == false)
	{
		m_ShaderEnvironment.SetShader(m_pDeviceContext.Get());
		RenderEnvironment();// 환경맵
	}
		
	m_ShaderBaseSkeletalMesh.SetShader(m_pDeviceContext.Get());
	RenderSkeletalMeshInstanceTranslucent();// 반투명

	m_ShaderBaseStaticMesh.SetShader(m_pDeviceContext.Get());
	RenderStaticMeshInstanceTranslucent();	

	RenderDebugDraw();
	RenderImGui();	
	ClearMeshInstance();
	m_pSwapChain->Present(0, 0);	// Present our back buffer to our front buffer
}

void D3DRenderManager::RenderDebugDraw()
{
	// 일반적인 메시그리는 RenderState가 아닌 디버깅용 RenderState를 사용한다.
	// *추후 PipelineObjectStates를 관리할 필요가 있겠다. 
	m_pDeviceContext->OMSetBlendState( DebugDraw::g_States->Opaque(), nullptr, 0xFFFFFFFF);	
	//m_pDeviceContext->RSSetState(DebugDraw::g_States->CullNone());

	DebugDraw::g_BatchEffect->Apply(m_pDeviceContext.Get());
	DebugDraw::g_BatchEffect->SetView(m_View);
	DebugDraw::g_BatchEffect->SetProjection(m_Projection);

	m_pDeviceContext->IASetInputLayout(DebugDraw::g_pBatchInputLayout.Get());

	DebugDraw::g_Batch->Begin();

	if (m_bDrawDebugCulling)
	{
		DebugDraw::Draw(DebugDraw::g_Batch.get(), m_Frustum, Colors::Yellow); // BoundingBox

		for (auto& SkeletalMeshComponent : m_SkeletalMeshComponents)
		{
			DebugDraw::Draw(DebugDraw::g_Batch.get(), SkeletalMeshComponent->m_BoundingBox,
				SkeletalMeshComponent->m_bIsCulled ? Colors::Red : Colors::Blue); // BoundingBox
		}
		for (auto& StaticMeshComponent : m_StaticMeshComponents)
		{
			DebugDraw::Draw(DebugDraw::g_Batch.get(), StaticMeshComponent->m_BoundingBox,
				StaticMeshComponent->m_bIsCulled ? Colors::Red : Colors::Blue); // BoundingBox
		}
	}

	if (m_bDrawDebugCollision)
	{
		const std::list<CollisionComponent*>& m_CollisionComponents = CollisionManager::Instance->m_CollisionComponents;
		for (auto& CollisionComponent : m_CollisionComponents)
		{
			switch (CollisionComponent->m_ColliderType)
			{
			case ColliderType::Sphere:
			{
				SphereComponent* pSphere = static_cast<SphereComponent*>(CollisionComponent);
				DebugDraw::Draw(DebugDraw::g_Batch.get(), pSphere->m_Geomety, pSphere->m_bIsOverlapped ? Colors::Red : Colors::Green);
			}
			break;
			case ColliderType::Box:
			{
				BoxComponent* pBox = static_cast<BoxComponent*>(CollisionComponent);
				DebugDraw::Draw(DebugDraw::g_Batch.get(), pBox->m_Geomety, pBox->m_bIsOverlapped ? Colors::Red : Colors::Green);
			}
			break;
			}
		}
	}


	for (auto& ray : m_DebugDrawLines)
	{
		BoundingBox BoxStart(ray.origin, Math::Vector3(0.3f, 0.3f, 0.3f));
		BoundingBox BoxEnd(ray.direction, Math::Vector3(0.3f, 0.3f, 0.3f));
		DebugDraw::Draw(DebugDraw::g_Batch.get(), BoxStart, Colors::Red);
		DebugDraw::Draw(DebugDraw::g_Batch.get(), BoxEnd, Colors::Blue);
		DebugDraw::DrawRay(DebugDraw::g_Batch.get(),ray.origin,ray.direction,ray.normalize,ray.color);
	}

	DebugDraw::g_Batch->End();	
}

void D3DRenderManager::RenderImGui()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	{
		std::string Title = __FUNCTION__;
		ImGui::Begin(Title.c_str());
		ImGui::Text("Keyboard: WASD-QE , Mouse: rotation on RightClick");

		ImGui::Text("Frame average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		std::string str;
		GetVideoMemoryInfo(str);
		ImGui::Text("VideoMemory: %s", str.c_str());
		GetSystemMemoryInfo(str);
		ImGui::Text("SystemMemory: %s", str.c_str());
		ImGui::Checkbox("m_bDrawDebugCulling", &m_bDrawDebugCulling);
		ImGui::Checkbox("m_bDrawDebugCollision", &m_bDrawDebugCollision);
		ImGui::Checkbox("Work Culling", &m_bWorkCulling);
	    ImGui::Checkbox("Freeze Culling", &m_bFreezeCulling);
		ImGui::Text("Count DrawComponents: %d ", m_nDrawComponentCount);

		ImGui::Text("Light");
		ImGui::SliderFloat3("LightDirection", (float*)&m_Light.Direction, -1.0f, 1.0f);
		ImGui::ColorEdit3("LightRadiance", (float*)&m_Light.Radiance);
		
		ImGui::Text(" ");
		ImGui::Text("Material Override");	
		ImGui::CheckboxFlags("UseMarterialOverride", &m_MaterialOverride.UseMarterialOverride, 1);
		ImGui::ColorEdit3("BaseColorOverride", (float*)&m_MaterialOverride.BaseColorOverride);		
		ImGui::SliderFloat("MatalnessOverride", (float*)&m_MaterialOverride.MetalnessOverride, 0.0f, 1.0f);
		ImGui::SliderFloat("RoughnessOverride", (float*)&m_MaterialOverride.RoughnessOverride, 0.0f, 1.0f);
		ImGui::Text(" ");

		ImGui::Text("IBL");		
		ImGui::CheckboxFlags("UseIBL", &m_Global.UseIBL, 1);	// bit flag 인듯
		ImGui::SliderFloat("AmbientOcculusion", &m_Global.AmbientOcclusion, 0.0f, 1.0f);

		ImGui::Text("Post");
		ImGui::CheckboxFlags("UseGammaCorrection", &m_Global.UseGammaCorrection,1);
		ImGui::SliderFloat("Gamma", &m_Global.Gamma, 1.0f, 2.2f);

		ImGui::Text("BackBuffer");
		ImGui::ColorEdit3("clear color", (float*)&m_ClearColor); // Edit 3 floats representing a color	

		AddDebugVector3ToImGuiWindow("EyePosition", m_Light.EyePosition);
		AddDebugVector3ToImGuiWindow("LookAt", m_LookAt);
	
		if (m_pCamera.expired() == false)
		{
			auto pCamera = m_pCamera.lock();
			AddDebugMatrixToImGuiWindow("CameraWorld", pCamera->m_World);
		}
		ImGui::End();	
		ImGui::Begin("Shadow");
		AddDebugVector3ToImGuiWindow("ShadowPosition", m_ShadowPos);
		AddDebugVector3ToImGuiWindow("ShadowLootAt", m_ShadowLootAt);
		//ImGui::SliderFloat3("ShadowPosition",&m_ShadowPos.x, 0.0f, 10000.0f);
		//ImGui::SliderFloat3("ShadowLootAt",&m_ShadowLootAt.x, 0.0f, 10000.0f);
		AddDebugVector3ToImGuiWindow("ShadowDir", m_ShadowDir);
		ImGui::SliderFloat("ShadowProjectionNear", (float*)&m_ShadowProjectionNearFar.x, 1.0f, 1000.0f);
		ImGui::SliderFloat("ShadowProjectionFar", (float*)&m_ShadowProjectionNearFar.y, 1000.0f, 100000.0f);
		ImGui::Image(m_pShadowMapSRV.Get(), ImVec2(256, 256));
		ImGui::End();
		for (auto ImguiRenderable : m_ImGuiRenders)
		{
			ImguiRenderable->ImGuiRender();
		}
	}
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


void D3DRenderManager::RenderSkeletalMeshInstanceOpaque()
{
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pCBTransformW.GetAddressOf());  //debugdraw에서 변경시켜서 설정한다.
	m_pDeviceContext->RSSetState(m_pRasterizerStateCW.Get());
	m_pDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);	// 설정해제 , 다른옵션은 기본값
	
	Material* pPrevMaterial = nullptr;
	for (const auto& meshInstance : m_SkeletalMeshInstanceOpaque)
	{
		// 머터리얼이 이전 머터리얼과 다를때만 파이프라인에 텍스쳐를 변경한다.
		if (pPrevMaterial != meshInstance->m_pMaterial)
		{
			ApplyMaterial(meshInstance->m_pMaterial);	// 머터리얼 적용
			pPrevMaterial = meshInstance->m_pMaterial;
		}
		// 행렬팔레트 업데이트						
		meshInstance->UpdateMatrixPallete(&m_MatrixPalette);
		m_cbMatrixPallete.SetData(m_pDeviceContext.Get(), m_MatrixPalette);

		// Draw
		meshInstance->Render(m_pDeviceContext.Get());
	}	
}

void D3DRenderManager::RenderStaticMeshInstanceOpaque()
{
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pCBTransformW.GetAddressOf()); //debugdraw에서 변경시켜서 설정한다.
	m_pDeviceContext->RSSetState(m_pRasterizerStateCW.Get());
	m_pDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);	// 설정해제 , 다른옵션은 기본값\

	Material* pPrevMaterial = nullptr;
	for (const auto& meshInstance : m_StaticMeshInstanceOpaque)
	{
		// 머터리얼이 이전 머터리얼과 다를때만 파이프라인에 텍스쳐를 변경한다.
		if (pPrevMaterial != meshInstance->m_pMaterial)
		{
			ApplyMaterial(meshInstance->m_pMaterial);	// 머터리얼 적용
			pPrevMaterial = meshInstance->m_pMaterial;
		}

		m_TransformW.mWorld = meshInstance->m_pNodeWorldTransform->Transpose();
		m_pDeviceContext->UpdateSubresource(m_pCBTransformW.Get(), 0, nullptr, &m_TransformW, 0, 0);

		// Draw
		meshInstance->Render(m_pDeviceContext.Get());
	}
}

void D3DRenderManager::RenderSkeletalMeshInstanceTranslucent()
{
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pCBTransformW.GetAddressOf());  //debugdraw에서 변경시켜서 설정한다.
	m_pDeviceContext->RSSetState(m_pRasterizerStateCW.Get());
	m_pDeviceContext->OMSetBlendState(m_pAlphaBlendState.Get(), nullptr, 0xffffffff); // 알파블렌드 상태설정 , 다른옵션은 기본값 


	Material* pPrevMaterial = nullptr;
	for (const auto& meshInstance : m_SkeletalMeshInstanceTranslucent)
	{
		// 머터리얼이 이전 머터리얼과 다를때만 파이프라인에 텍스쳐를 변경한다.
		if (pPrevMaterial != meshInstance->m_pMaterial)
		{
			ApplyMaterial(meshInstance->m_pMaterial);	// 머터리얼 적용
			pPrevMaterial = meshInstance->m_pMaterial;
		}
		// 행렬팔레트 업데이트						
		meshInstance->UpdateMatrixPallete(&m_MatrixPalette);
		m_cbMatrixPallete.SetData(m_pDeviceContext.Get(), m_MatrixPalette);

		// Draw
		meshInstance->Render(m_pDeviceContext.Get());
	}
	
}

void D3DRenderManager::RenderStaticMeshInstanceTranslucent()
{
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pCBTransformW.GetAddressOf()); //debugdraw에서 변경시켜서 설정한다.
	m_pDeviceContext->RSSetState(m_pRasterizerStateCW.Get());
	m_pDeviceContext->OMSetBlendState(m_pAlphaBlendState.Get(), nullptr, 0xffffffff); // 알파블렌드 상태설정 , 다른옵션은 기본값 	

	Material* pPrevMaterial = nullptr;
	for (const auto& meshInstance : m_StaticMeshInstanceTranslucent)
	{
		// 머터리얼이 이전 머터리얼과 다를때만 파이프라인에 텍스쳐를 변경한다.
		if (pPrevMaterial != meshInstance->m_pMaterial)
		{
			ApplyMaterial(meshInstance->m_pMaterial);	// 머터리얼 적용
			pPrevMaterial = meshInstance->m_pMaterial;
		}

		m_TransformW.mWorld = meshInstance->m_pNodeWorldTransform->Transpose();
		m_pDeviceContext->UpdateSubresource(m_pCBTransformW.Get(), 0, nullptr, &m_TransformW, 0, 0);

		// Draw
		meshInstance->Render(m_pDeviceContext.Get());
	}

}

void D3DRenderManager::RenderEnvironment()
{
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pCBTransformW.GetAddressOf()); //debugdraw에서 변경시켜서 설정한다.
	m_pDeviceContext->RSSetState(m_pRasterizerStateCCW.Get());
	m_pDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);	// 설정해제 , 다른옵션은 기본값

	auto component = m_pEnvironmentMeshComponent.lock();	
	m_TransformW.mWorld = component->m_World.Transpose();
	m_pDeviceContext->UpdateSubresource(m_pCBTransformW.Get(), 0, nullptr, &m_TransformW, 0, 0);
	component->m_MeshInstance.Render(m_pDeviceContext.Get());
}

void D3DRenderManager::RenderShadowDepth()
{	

}

Microsoft::WRL::ComPtr<ID3D11SamplerState> D3DRenderManager::CreateSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode) const
{
	D3D11_SAMPLER_DESC desc = {};
	desc.Filter = filter;
	desc.AddressU = addressMode;
	desc.AddressV = addressMode;
	desc.AddressW = addressMode;
	desc.MaxAnisotropy = (filter == D3D11_FILTER_ANISOTROPIC) ? D3D11_REQ_MAXANISOTROPY : 1;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	ComPtr<ID3D11SamplerState> samplerState;
	HR_T(m_pDevice->CreateSamplerState(&desc, &samplerState));
	return samplerState;
}

HRESULT D3DRenderManager::CreateTextureFromFile(const wchar_t* szFileName, ID3D11ShaderResourceView** textureView, ID3D11Resource** texture)
{
	std::filesystem::path path(szFileName);
	std::wstring strExtension = path.extension();
	std::transform(strExtension.begin(), strExtension.end(), strExtension.begin(), ::towlower);

	DirectX::TexMetadata metadata1;
	DirectX::ScratchImage scratchImage;

	HRESULT hr = S_OK;
	if (strExtension == L".dds")
	{
		HR_T(hr = DirectX::LoadFromDDSFile(szFileName, DirectX::DDS_FLAGS_NONE, &metadata1, scratchImage));
	}
	else if (strExtension == L".tga")
	{
		HR_T(hr = DirectX::LoadFromTGAFile(szFileName, &metadata1, scratchImage));
	}
	else if (strExtension == L".hdr")
	{
		HR_T(hr = DirectX::LoadFromHDRFile(szFileName, &metadata1, scratchImage));
	}
	else // 기타..
	{
		HR_T(hr = DirectX::LoadFromWICFile(szFileName, DirectX::WIC_FLAGS_NONE, &metadata1, scratchImage));
	}

	HR_T(hr = DirectX::CreateShaderResourceView(m_pDevice, scratchImage.GetImages(), scratchImage.GetImageCount(), metadata1, textureView));
	return hr;
}

HRESULT D3DRenderManager::CreateSamplerStateE(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, ID3D11SamplerState** ppSamplerState)
{
	D3D11_SAMPLER_DESC desc = {};
	desc.Filter = filter;
	desc.AddressU = addressMode;
	desc.AddressV = addressMode;
	desc.AddressW = addressMode;
	desc.MaxAnisotropy = (filter == D3D11_FILTER_ANISOTROPIC) ? D3D11_REQ_MAXANISOTROPY : 1;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	return m_pDevice->CreateSamplerState(&desc, ppSamplerState);
}

FrameBuffer D3DRenderManager::CreateFrameBuffer(UINT width, UINT height, UINT samples, DXGI_FORMAT renderTargetFormat, DXGI_FORMAT depthstencilFormat)
{
	FrameBuffer fb;
	fb.Width = width;
	fb.Height = height;
	fb.Samples = samples;

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = samples;

	if (renderTargetFormat != DXGI_FORMAT_UNKNOWN) {
		desc.Format = renderTargetFormat;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET;
		if (samples <= 1) {
			desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		}
		if (FAILED(m_pDevice->CreateTexture2D(&desc, nullptr, &fb.RenderTargetTexture))) {
			throw std::runtime_error("Failed to create FrameBuffer color texture");
		}

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = desc.Format;
		rtvDesc.ViewDimension = (samples > 1) ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
		if (FAILED(m_pDevice->CreateRenderTargetView(fb.RenderTargetTexture.Get(), &rtvDesc, &fb.RTV))) {
			throw std::runtime_error("Failed to create FrameBuffer render target view");
		}

		if (samples <= 1) {
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = desc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = 1;
			if (FAILED(m_pDevice->CreateShaderResourceView(fb.RenderTargetTexture.Get(), &srvDesc, &fb.SRV))) {
				throw std::runtime_error("Failed to create FrameBuffer shader resource view");
			}
		}
	}

	if (depthstencilFormat != DXGI_FORMAT_UNKNOWN) {
		desc.Format = depthstencilFormat;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		if (FAILED(m_pDevice->CreateTexture2D(&desc, nullptr, &fb.DepthStencilTexture))) {
			throw std::runtime_error("Failed to create FrameBuffer depth-stencil texture");
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = desc.Format;
		dsvDesc.ViewDimension = (samples > 1) ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
		if (FAILED(m_pDevice->CreateDepthStencilView(fb.DepthStencilTexture.Get(), &dsvDesc, &fb.DSV))) {
			throw std::runtime_error("Failed to create FrameBuffer depth-stencil view");
		}
	}

	return fb;
}

void D3DRenderManager::SetBaseViewPort(UINT Width, UINT Height)
{
	m_BaseViewport = {};
	m_BaseViewport.TopLeftX = 0;
	m_BaseViewport.TopLeftY = 0;
	m_BaseViewport.Width = (float)Width;
	m_BaseViewport.Height = (float)Height;
	m_BaseViewport.MinDepth = 0.0f;
	m_BaseViewport.MaxDepth = 1.0f;
}

void D3DRenderManager::SetShadowViewPort(UINT Width, UINT Height)
{
	m_ShadowViewport = {};
	m_ShadowViewport.TopLeftX = 0;
	m_ShadowViewport.TopLeftY = 0;
	m_ShadowViewport.Width = float(Width);
	m_ShadowViewport.Height = float(Height);
	m_ShadowViewport.MinDepth = 0.0f;
	m_ShadowViewport.MaxDepth = 1.0f;
}

void D3DRenderManager::MSAACheck(DXGI_FORMAT format, UINT& SampleCount, UINT& Quality) {

	// Check for MSAA Support
	UINT NumQuality = 0;
	UINT Samples = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;

	// Find max number of samples and quality level
	for (; Samples > 1; Samples /= 2) {
		m_pDevice->CheckMultisampleQualityLevels(format, Samples, &NumQuality);
		if (NumQuality > 0) { break; }
	}

	// Update
	if (NumQuality > 0) {
		SampleCount = Samples;
		Quality = NumQuality-1;

		std::cout << "Found MSAA Quality and Sample Count combination (Samples="
			<< SampleCount << "; Quality=" << Quality << ")" << std::endl;
	}
	else {
		std::cout << "Failed to find MSAA Quality and Sample Count combination" << std::endl;
	}

}

void D3DRenderManager::CreateBuffers()
{
	// 4. 렌더타겟뷰 생성.  (백버퍼를 이용하는 렌더타겟뷰)	
	ComPtr<ID3D11Texture2D> pBackBufferTexture;
	HR_T(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pBackBufferTexture.GetAddressOf()));
	HR_T(m_pDevice->CreateRenderTargetView(pBackBufferTexture.Get(), NULL, m_pBackBufferRTV.GetAddressOf()));  // 텍스처는 내부 참조 증가

	//6. 뎊스&스텐실 뷰 생성
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = (UINT)m_BaseViewport.Width;
	descDepth.Height = (UINT)m_BaseViewport.Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;	
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> textureDepthStencil;
	HR_T(m_pDevice->CreateTexture2D(&descDepth, nullptr, textureDepthStencil.GetAddressOf()));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	HR_T(m_pDevice->CreateDepthStencilView(textureDepthStencil.Get(), &descDSV, m_pDefaultDSV.GetAddressOf()));

	//m_ShadowBuffer = CreateFrameBuffer(m_Viewport.Width, m_Viewport.Height, 1, DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_D24_UNORM_S8_UINT);
	//m_pDeviceContext->PSSetShaderResources(11, 1, m_ShadowBuffer.SRV.GetAddressOf()); // 한번에 7개의 텍스처를 설정한다.

	{
		
		//create shadow map texture desc
		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = m_ShadowViewport.Width;
		texDesc.Height = m_ShadowViewport.Height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		HR_T(m_pDevice->CreateTexture2D(&texDesc, NULL, m_pShadowMap.GetAddressOf()));

		// Create the depth stencil view desc
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		HR_T(m_pDevice->CreateDepthStencilView(m_pShadowMap.Get(), &descDSV, m_pShadowMapDSV.GetAddressOf()));


		//create shader resource view desc
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;	
		HR_T(m_pDevice->CreateShaderResourceView(m_pShadowMap.Get(), &srvDesc, m_pShadowMapSRV.GetAddressOf()));

		m_pDeviceContext->PSSetShaderResources(11, 1, m_pShadowMapSRV.GetAddressOf()); // 한번에 7개의 텍스처를 설정한다.
	}

}

void D3DRenderManager::CreateShaders()
{
	HRESULT hr;
	D3D_SHADER_MACRO macroVertexSkinning[] =
	{
		{"VERTEX_SKINNING",""}, // 매크로 이름과 값을 설정
		{nullptr, nullptr}    // 배열의 끝을 나타내기 위해 nullptr로 끝낸다.
	};
	D3D11_INPUT_ELEMENT_DESC layoutSkeletalMesh[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES" , 0, DXGI_FORMAT_R32G32B32A32_SINT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHTS" , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D11_INPUT_ELEMENT_DESC layoutStaticMesh[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ComPtr<ID3D10Blob> buffer = nullptr;
		
	//Base 
	{
		//1. SkeletalMesh  PBR VertexShader
		buffer.Reset();
		HR_T(CompileShaderFromFile(L"../Resource/VS_PBR.hlsl", macroVertexSkinning, "main", "vs_5_0", buffer.GetAddressOf()));

		hr = m_pDevice->CreateInputLayout(layoutSkeletalMesh, ARRAYSIZE(layoutSkeletalMesh),
			buffer->GetBufferPointer(), buffer->GetBufferSize(),m_ShaderBaseSkeletalMesh.IL.GetAddressOf());

		HR_T(m_pDevice->CreateVertexShader(buffer->GetBufferPointer(),
			buffer->GetBufferSize(), NULL, m_ShaderBaseSkeletalMesh.VS.GetAddressOf()));
	   
		// 2. StaticMesh  PBR VertexShader	
		buffer.Reset();
		HR_T(CompileShaderFromFile(L"../Resource/VS_PBR.hlsl", nullptr, "main", "vs_5_0", buffer.GetAddressOf()));

		HR_T(m_pDevice->CreateInputLayout(layoutStaticMesh, ARRAYSIZE(layoutStaticMesh),
			buffer->GetBufferPointer(), buffer->GetBufferSize(), m_ShaderBaseStaticMesh.IL.GetAddressOf()));

		HR_T(m_pDevice->CreateVertexShader(buffer->GetBufferPointer(),
			buffer->GetBufferSize(), NULL, m_ShaderBaseStaticMesh.VS.GetAddressOf()));

		// 3. PBR PixelShader
		buffer.Reset();
		HR_T(CompileShaderFromFile(L"../Resource/PS_PBR.hlsl", nullptr, "main", "ps_5_0", buffer.GetAddressOf()));
		ComPtr<ID3D11PixelShader> ps;
		HR_T(m_pDevice->CreatePixelShader(buffer->GetBufferPointer(),
			buffer->GetBufferSize(), NULL, ps.GetAddressOf()));

		m_ShaderBaseStaticMesh.PS = ps;
		m_ShaderBaseSkeletalMesh.PS = ps;
	}

	// 그림자용 셰이더 1.  SkeletalMesh  PBR VertexShader
	{
		buffer.Reset();
		HR_T(CompileShaderFromFile(L"../Resource/VS_ShadowDepth.hlsl", macroVertexSkinning, "main", "vs_5_0", buffer.GetAddressOf()));

		hr = m_pDevice->CreateInputLayout(layoutSkeletalMesh, ARRAYSIZE(layoutSkeletalMesh),
			buffer->GetBufferPointer(), buffer->GetBufferSize(), m_ShaderShadowSkeletalMesh.IL.GetAddressOf());

		HR_T(m_pDevice->CreateVertexShader(buffer->GetBufferPointer(),
			buffer->GetBufferSize(), NULL, m_ShaderShadowSkeletalMesh.VS.GetAddressOf()));

		// 그림자용 셰이더  2. StaticMesh  PBR VertexShader	
		buffer.Reset();
		HR_T(CompileShaderFromFile(L"../Resource/VS_ShadowDepth.hlsl", nullptr, "main", "vs_5_0", buffer.GetAddressOf()));
		
		HR_T(m_pDevice->CreateInputLayout(layoutStaticMesh, ARRAYSIZE(layoutStaticMesh),
			buffer->GetBufferPointer(), buffer->GetBufferSize(), m_ShaderShadowStaticMesh.IL.GetAddressOf()));

		HR_T(m_pDevice->CreateVertexShader(buffer->GetBufferPointer(),
			buffer->GetBufferSize(), NULL, m_ShaderShadowStaticMesh.VS.GetAddressOf()));

		// 그림자용 셰이더  3. PBR PixelShader
		buffer.Reset();
		HR_T(CompileShaderFromFile(L"../Resource/PS_ShadowDepth.hlsl", nullptr, "main", "ps_5_0", buffer.GetAddressOf()));
		ComPtr<ID3D11PixelShader> ps;
		HR_T(m_pDevice->CreatePixelShader(buffer->GetBufferPointer(),
			buffer->GetBufferSize(), NULL, ps.GetAddressOf()));

		m_ShaderShadowStaticMesh.PS = ps;
		m_ShaderShadowSkeletalMesh.PS = ps;
	}

	// Environment
	{
		buffer.Reset();
		HR_T(CompileShaderFromFile(L"../Resource/VS_Environment.hlsl", nullptr, "main", "vs_5_0", buffer.GetAddressOf()));
		HR_T(m_pDevice->CreateVertexShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), NULL,m_ShaderEnvironment.VS.GetAddressOf()));
		
		HR_T(m_pDevice->CreateInputLayout(layoutStaticMesh, ARRAYSIZE(layoutStaticMesh),
			buffer->GetBufferPointer(), buffer->GetBufferSize(), m_ShaderEnvironment.IL.GetAddressOf()));

		buffer.Reset();
		HR_T(CompileShaderFromFile(L"../Resource/PS_Environment.hlsl", nullptr, "main", "ps_5_0", buffer.GetAddressOf()));
		HR_T(m_pDevice->CreatePixelShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), NULL, m_ShaderEnvironment.PS.GetAddressOf()));
	}

}

void D3DRenderManager::ApplyMaterial(Material* pMaterial)
{		
	ID3D11ShaderResourceView* pNullSRV[7] = { 
		pMaterial->m_pBaseColor != nullptr ? pMaterial->m_pBaseColor->m_pTextureSRV.Get() : nullptr,
		pMaterial->m_pNormal != nullptr ? pMaterial->m_pNormal->m_pTextureSRV.Get() : nullptr,
		pMaterial->m_pSpecular != nullptr ? pMaterial->m_pSpecular->m_pTextureSRV.Get() : nullptr,
		pMaterial->m_pEmissive != nullptr ? pMaterial->m_pEmissive->m_pTextureSRV.Get() : nullptr,
		pMaterial->m_pOpacity != nullptr ? pMaterial->m_pOpacity->m_pTextureSRV.Get() : nullptr,
		pMaterial->m_pMetalness != nullptr ? pMaterial->m_pMetalness->m_pTextureSRV.Get() : nullptr,
		pMaterial->m_pRoughness != nullptr ? pMaterial->m_pRoughness->m_pTextureSRV.Get() : nullptr ,
		};
	
	m_pDeviceContext->PSSetShaderResources(0, 7, pNullSRV); // 한번에 7개의 텍스처를 설정한다.

	m_CpuCbMaterial.UseBaseColorMap = pMaterial->m_pBaseColor != nullptr ? true : false;
	m_CpuCbMaterial.UseNormalMap = pMaterial->m_pNormal != nullptr ? true : false;
	m_CpuCbMaterial.UseSpecularMap = pMaterial->m_pSpecular != nullptr ? true : false;
	m_CpuCbMaterial.UseEmissiveMap = pMaterial->m_pEmissive != nullptr ? true : false;
	m_CpuCbMaterial.UseOpacityMap = pMaterial->m_pOpacity != nullptr ? true : false;
	m_CpuCbMaterial.UseMetalnessMap = pMaterial->m_pMetalness != nullptr ? true : false;
	m_CpuCbMaterial.UseRoughnessMap = pMaterial->m_pRoughness != nullptr ? true : false;	

	m_pDeviceContext->UpdateSubresource(m_pCBMaterial.Get(), 0, nullptr, &m_CpuCbMaterial, 0, 0);
}

void  D3DRenderManager::GetVideoMemoryInfo(std::string& out)
{
	DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
	m_pDXGIAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo);

	out = std::to_string(videoMemoryInfo.CurrentUsage / 1024 / 1024) + " MB" + " / " + std::to_string(videoMemoryInfo.Budget / 1024 / 1024) + " MB";
}

void D3DRenderManager::GetSystemMemoryInfo(std::string& out)
{
	HANDLE hProcess = GetCurrentProcess();	
	PROCESS_MEMORY_COUNTERS_EX pmc;
	pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);
	GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	out = std::to_string( (pmc.PagefileUsage) / 1024 / 1024) + " MB";	
}

std::weak_ptr<EnvironmentMeshComponent> D3DRenderManager::GetEnvironmentMeshComponent() const
{
	return m_pEnvironmentMeshComponent;
}

void D3DRenderManager::SetEnvironment(std::weak_ptr<EnvironmentMeshComponent> val)
{
	m_pEnvironmentMeshComponent = val;
	auto component = m_pEnvironmentMeshComponent.lock();
	// Shared.hlsli 에서 텍스처 slot7 확인
	m_pDeviceContext->PSSetShaderResources(7, 1, component->m_EnvironmentTextureResource->m_pTextureSRV.GetAddressOf()); 
	m_pDeviceContext->PSSetShaderResources(8, 1, component->m_IBLDiffuseTextureResource->m_pTextureSRV.GetAddressOf());
	m_pDeviceContext->PSSetShaderResources(9, 1, component->m_IBLSpecularTextureResource->m_pTextureSRV.GetAddressOf());
	m_pDeviceContext->PSSetShaderResources(10, 1, component->m_IBLBRDFTextureResource->m_pTextureSRV.GetAddressOf());	
	m_Global.UseIBL = true;
	m_pDeviceContext->UpdateSubresource(m_pCBGlobal.Get(), 0, nullptr, &m_Global, 0, 0);
}

void D3DRenderManager::AddDebugStringToImGuiWindow(const std::string& header,const std::string& str)
{
	ImGui::Text(" ");
	ImGui::Text(header.c_str());
	ImGui::Text(str.c_str());	
}

void D3DRenderManager::AddDebugMatrixToImGuiWindow(const std::string& header, const Matrix& mat)
{
	ImGui::Text(" ");
	ImGui::Text(header.c_str());
	ImGui::Text("%f, %f, %f, %f", mat._11, mat._12, mat._13, mat._14);
	ImGui::Text("%f, %f, %f, %f", mat._21, mat._22, mat._23, mat._24);
	ImGui::Text("%f, %f, %f, %f", mat._31, mat._32, mat._33, mat._34);
	ImGui::Text("%f, %f, %f, %f", mat._41, mat._42, mat._43, mat._44);	
}	

void D3DRenderManager::AddDebugFloatToImGuiWindow(const std::string& header, const float& value)
{
	ImGui::Text(" ");
	ImGui::Text(header.c_str());
	ImGui::Text("%f", value);
}

void D3DRenderManager::AddSkeletalMeshComponent(SkeletalMeshComponent* pSkeletalMeshComponent)
{
	pSkeletalMeshComponent->m_ItRenderManager = m_SkeletalMeshComponents.insert(m_SkeletalMeshComponents.end(), pSkeletalMeshComponent);
}

void D3DRenderManager::RemoveSkeletalMeshComponent(SkeletalMeshComponent* pSkeletalMeshComponent)
{
	m_SkeletalMeshComponents.erase(pSkeletalMeshComponent->m_ItRenderManager);

}

void D3DRenderManager::AddStaticMeshComponent(StaticMeshComponent* pStaticMeshComponent)
{
	pStaticMeshComponent->m_ItRenderManager = m_StaticMeshComponents.insert(m_StaticMeshComponents.end(), pStaticMeshComponent);
}

void D3DRenderManager::RemoveStaticMeshComponent(StaticMeshComponent* pStaticMeshComponent)
{
	m_StaticMeshComponents.erase(pStaticMeshComponent->m_ItRenderManager);
}

void D3DRenderManager::AddImguiRenderable(IImGuiRenderable* pIImGuiRenderable)
{
	m_ImGuiRenders.push_back(pIImGuiRenderable);
}

void D3DRenderManager::RemoveImguiRenderable(IImGuiRenderable* pIImGuiRenderable)
{
	m_ImGuiRenders.remove(pIImGuiRenderable);
}


void D3DRenderManager::CreateMousePickingRay(float mouseX, float mouseY, Math::Vector3& rayOrigin, Math::Vector3& rayDirection)
{
	// Screen공간의 마우스 위치를 NDC 좌표계( -1 ~  +1)로 변경
	float viewX = (+2.0f * mouseX / m_BaseViewport.Width - 1.0f) / m_Projection(0, 0);   // 투영행렬에 반영된 뷰포트 종횡비 고려하여 점을 조정
	float viewY = (-2.0f * mouseY / m_BaseViewport.Height + 1.0f) / m_Projection(1, 1);  // 투영행렬에 반영된 뷰포트 종횡비 고려하여 점을 조정

	// 카메라 공간의 좌표와 벡터
	Vector3 org = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 dir = Vector3(viewX, viewY, 1.0f);

	// 월드 공간의 좌표와 벡터
	auto cam = m_pCamera.lock();
	rayOrigin = Vector3::Transform(org, cam->m_World);
	rayDirection = Vector3::TransformNormal(dir, cam->m_World);
	rayDirection.Normalize();
}	

void D3DRenderManager::AddDebugDrawLine(const Math::Vector3& origin, const Math::Vector3& direction, bool normalize, const Math::Vector3& color, float time)
{
	DebugRay debugDrawLine;
	debugDrawLine.origin = origin;
	debugDrawLine.direction = direction;
	debugDrawLine.normalize = normalize;
	debugDrawLine.color = color;
	debugDrawLine.time = time;
	m_DebugDrawLines.push_back(debugDrawLine);
}

void D3DRenderManager::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC bd = {};
	bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CB_TransformW);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, m_pCBTransformW.GetAddressOf()));

	bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CB_TransformVP);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, m_pCBTransformVP.GetAddressOf()));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CB_DirectionLight);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, m_pCBDirectionLight.GetAddressOf()));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CB_Marterial);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, m_pCBMaterial.GetAddressOf()));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CB_Global);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, m_pCBGlobal.GetAddressOf()));

	m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pCBTransformW.GetAddressOf());
	m_pDeviceContext->VSSetConstantBuffers(1, 1, m_pCBTransformVP.GetAddressOf());
	m_pDeviceContext->VSSetConstantBuffers(2, 1, m_pCBDirectionLight.GetAddressOf());
	// 3 material
	m_cbMatrixPallete.Create(m_pDevice);
	auto buffer = m_cbMatrixPallete.GetBuffer();
	m_pDeviceContext->VSSetConstantBuffers(4, 1, &buffer);

	// 픽셀셰이더 상수설정
	m_pDeviceContext->PSSetConstantBuffers(0, 1, m_pCBTransformW.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(1, 1, m_pCBTransformVP.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(2, 1, m_pCBDirectionLight.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(3, 1, m_pCBMaterial.GetAddressOf());

	m_pDeviceContext->PSSetConstantBuffers(5, 1, m_pCBGlobal.GetAddressOf());
	m_pDeviceContext->UpdateSubresource(m_pCBGlobal.Get(), 0, nullptr, &m_Global, 0, 0);

	m_cbMaterialOverride.Create(m_pDevice);
	{
		auto buffer = m_cbMaterialOverride.GetBuffer();
		m_pDeviceContext->PSSetConstantBuffers(6, 1, &buffer);
	}

}

void D3DRenderManager::CreateSamplerState()
{
	// 7. 텍스처 샘플러 생성
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR_T(m_pDevice->CreateSamplerState(&sampDesc, m_pSamplerLinear.GetAddressOf()));	

	sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.MaxAnisotropy = (sampDesc.Filter == D3D11_FILTER_ANISOTROPIC) ? D3D11_REQ_MAXANISOTROPY : 1;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR_T(m_pDevice->CreateSamplerState(&sampDesc, m_pSamplerClamp.GetAddressOf()));

	m_pDeviceContext->PSSetSamplers(0, 1, m_pSamplerLinear.GetAddressOf());
	m_pDeviceContext->PSSetSamplers(1, 1, m_pSamplerClamp.GetAddressOf());	
}

void D3DRenderManager::CreateRasterizerState()
{
	HRESULT hr = 0;

	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.AntialiasedLineEnable = true;
	rasterizerDesc.MultisampleEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.DepthClipEnable = true;
	HR_T(m_pDevice->CreateRasterizerState(&rasterizerDesc, m_pRasterizerStateCCW.GetAddressOf()));

	rasterizerDesc.FrontCounterClockwise = false;
	HR_T(m_pDevice->CreateRasterizerState(&rasterizerDesc, m_pRasterizerStateCW.GetAddressOf()));
}

void D3DRenderManager::CreateBlendState()
{
	//7. 알파블렌딩을 위한 블렌드 상태 생성
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.IndependentBlendEnable = false;
	D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
	rtBlendDesc.BlendEnable = true; // 블렌드 사용 여부
	// FinalRGB = SrcRGB *SrcBlend + DestRGB*DestBlend
	rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	rtBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;	// SrcBlend는 SrcColor의 알파값
	rtBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	// DestBlend는 (1-SourceColor.a)
	// FinalAlpha = (SrcAlpha * SrcBlendAlpha) + (DestAlpha * DestBlendAlpha)
	rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;	// SrcBlendAlpha = 1
	rtBlendDesc.DestBlendAlpha = D3D11_BLEND_ONE;	// DestBlendAlpha = 1	
	rtBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 렌더타겟에 RGBA 모두 Write
	blendDesc.RenderTarget[0] = rtBlendDesc;
	HR_T(m_pDevice->CreateBlendState(&blendDesc, m_pAlphaBlendState.GetAddressOf()));
}

void D3DRenderManager::CreateEnvironment()
{
	
}

void D3DRenderManager::AddDebugVector4ToImGuiWindow(const std::string& header, const Vector4& value)
{
	ImGui::Text(" ");
	ImGui::Text(header.c_str());
	ImGui::Text("%f, %f, %f, %f", value.x, value.y, value.z, value.w);

}

void D3DRenderManager::AddDebugVector3ToImGuiWindow(const std::string& header, const Vector3& value)
{
	ImGui::Text(" ");
	ImGui::Text(header.c_str());
	ImGui::Text("%f, %f, %f", value.x, value.y, value.z);
}

void D3DRenderManager::AddDebugVector2ToImGuiWindow(const std::string& header, const Vector2& value)
{
	ImGui::Text(" ");
	ImGui::Text(header.c_str());
	ImGui::Text("%f, %f", value.x, value.y);
}

