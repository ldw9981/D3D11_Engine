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

#include "DebugDraw.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")



D3DRenderManager* D3DRenderManager::Instance = nullptr;

ID3D11Device* D3DRenderManager::m_pDevice = nullptr;

D3DRenderManager::D3DRenderManager()
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
		m_SkeletalMeshInstance.push_back(&pModel->m_MeshInstances[i]);
	}
}

void D3DRenderManager::AddMeshInstance(StaticMeshComponent* pModel)
{
	for (size_t i = 0; i < pModel->m_MeshInstances.size(); i++)
	{
		m_StaticMeshInstance.push_back(&pModel->m_MeshInstances[i]);
	}
}

bool D3DRenderManager::Initialize(HWND Handle,UINT Width, UINT Height)
{
	m_hWnd = Handle;
	HRESULT hr = 0;	// �����.

	// Create DXGI factory
	HR_T(CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)m_pDXGIFactory.GetAddressOf()));
	HR_T(m_pDXGIFactory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(m_pDXGIAdapter.GetAddressOf())));
	


	// ����ü�� �Ӽ� ���� ����ü ����.
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = Handle;	// ����ü�� ����� â �ڵ� ��.
	swapDesc.Windowed = true;		// â ��� ���� ����.
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// �����(�ؽ�ó)�� ����/���� ũ�� ����.
	swapDesc.BufferDesc.Width = Width;
	swapDesc.BufferDesc.Height = Height;
	// ȭ�� �ֻ��� ����.
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	// ���ø� ���� ����.
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// 1. ��ġ ����.   2.����ü�� ����. 3.��ġ ���ؽ�Ʈ ����.

	HR_T(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, 0,
		D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext));

	// 4. ����Ÿ�ٺ� ����.  (����۸� �̿��ϴ� ����Ÿ�ٺ�)	
	ID3D11Texture2D* pBackBufferTexture = nullptr;
	HR_T(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture));
	HR_T(m_pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &m_pRenderTargetView));  // �ؽ�ó�� ���� ���� ����
	SAFE_RELEASE(pBackBufferTexture);	//�ܺ� ���� ī��Ʈ�� ���ҽ�Ų��.
	// ���� Ÿ���� ���� ��� ���������ο� ���ε��մϴ�.
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

	//5. ����Ʈ ����.	
	m_Viewport = {};
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = (float)Width;
	m_Viewport.Height = (float)Height;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_pDeviceContext->RSSetViewports(1, &m_Viewport);

	//6. �X��&���ٽ� �� ����
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = Width;
	descDepth.Height = Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ID3D11Texture2D* textureDepthStencil = nullptr;
	HR_T(m_pDevice->CreateTexture2D(&descDepth, nullptr, &textureDepthStencil));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	HR_T(m_pDevice->CreateDepthStencilView(textureDepthStencil, &descDSV, &m_pDepthStencilView));
	SAFE_RELEASE(textureDepthStencil);

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);


	//7. ���ĺ����� ���� ���� ���� ����
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
	rtBlendDesc.BlendEnable = true; // ���� ��� ����
	// FinalRGB = SrcRGB *SrcBlend + DestRGB*DestBlend
	rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	rtBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;	// SrcBlend�� SrcColor�� ���İ�
	rtBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	// DestBlend�� (1-SourceColor.a)
	// FinalAlpha = (SrcAlpha * SrcBlendAlpha) + (DestAlpha * DestBlendAlpha)
	rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;	// SrcBlendAlpha = 1
	rtBlendDesc.DestBlendAlpha = D3D11_BLEND_ONE;	// DestBlendAlpha = 1	
	rtBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // ����Ÿ�ٿ� RGBA ��� Write
	blendDesc.RenderTarget[0] = rtBlendDesc;
	HR_T(m_pDevice->CreateBlendState(&blendDesc, &m_pAlphaBlendState));

	/*
		ImGui �ʱ�ȭ.
	*/
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();


	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX11_Init(this->m_pDevice, this->m_pDeviceContext);

	// * Render() ���� ���������ο� ���ε��� ���ؽ� ���̴� ����
	CreateSkeletalMesh_VS_IL();
	CreateStaticMesh_VS_IL();

	// * Render() ���� ���������ο� ���ε��� �ȼ� ���̴� ����
	CreatePS();


	// 6. Render() ���� ���������ο� ���ε��� ��� ���� ����
	// Create the constant buffer

	D3D11_BUFFER_DESC bd = {};
	bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CB_TransformW);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, &m_pCBTransformW));

	bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CB_TransformVP);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, &m_pCBTransformVP));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CB_DirectionLight);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, &m_pCBDirectionLight));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CB_Marterial);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, &m_pGpuCbMaterial));

	m_cbMatrixPallete.Create(m_pDevice);

	// 7. �ؽ�ó ���÷� ����
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR_T(m_pDevice->CreateSamplerState(&sampDesc, &m_pSamplerLinear));


	// Initialize the world matrix
	m_World = XMMatrixIdentity();
	XMVECTOR Eye = XMVectorSet(0.0f, 0.0f, -1000.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_View = XMMatrixLookAtLH(Eye, At, Up);
	m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_Viewport.Width / (FLOAT)m_Viewport.Height, 1.0f, 10000.0f);

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

	//	SAFE_RELEASE(m_cbMatrixPallete.GetBuffer()); //ComPtr
	SAFE_RELEASE(m_pCBDirectionLight);
	SAFE_RELEASE(m_pCBTransformW);
	SAFE_RELEASE(m_pCBTransformVP);
	SAFE_RELEASE(m_pGpuCbMaterial);
	SAFE_RELEASE(m_pAlphaBlendState);
	SAFE_RELEASE(m_pSamplerLinear);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pSkeletalMeshInputLayout);
	SAFE_RELEASE(m_pSkeletalMeshVertexShader);

	SAFE_RELEASE(m_pStaticMeshInputLayout);
	SAFE_RELEASE(m_pStaticMeshVertexShader);

	// Cleanup DirectX
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDeviceContext);
#ifdef DEBUG_D3D11_LIVEDEVICE
	ID3D11Debug* pD3D11Dbug = nullptr;
	HRESULT hr;
	hr = m_pDevice->QueryInterface(IID_PPV_ARGS(&pD3D11Dbug));
	pD3D11Dbug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	pD3D11Dbug->Release();
#endif // DEBUG_D3D11_LIVEDEVICE
	SAFE_RELEASE(m_pDevice);
}



void D3DRenderManager::Update()
{	
	if (m_pCamera.expired() == false)
	{
		auto pCamera = m_pCamera.lock();
		Math::Vector3 eye = pCamera->m_World.Translation();
		m_LookAt = pCamera->m_World.Translation() + -pCamera->m_World.Forward();
		Math::Vector3 up = pCamera->m_World.Up();
		m_View = XMMatrixLookAtLH(eye, m_LookAt, up);
		SetEyePosition(eye);
	}

	for (auto& SkeletalMeshComponent : m_SkeletalMeshComponents)
	{
		AddMeshInstance(SkeletalMeshComponent);
	}

	for (auto& StaticMeshComponent : m_StaticMeshComponents)
	{
		AddMeshInstance(StaticMeshComponent);
	}
}

void D3DRenderManager::Render()
{
	// Clear the back buffer
	const float clear_color_with_alpha[4] = { m_ClearColor.x , m_ClearColor.y , m_ClearColor.z, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, clear_color_with_alpha);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	
	m_pDeviceContext->RSSetState(nullptr);
	

	// ���ؽ����̴� ���� (StaticMesh-SkeletalMesh�� ���� Render������ �ٲ��)
	// ���ؽ� ���̴� ��� ����
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pCBTransformW);
	m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pCBTransformVP);
	m_pDeviceContext->VSSetConstantBuffers(2, 1, &m_pCBDirectionLight);
	// 3 material
	auto buffer = m_cbMatrixPallete.GetBuffer();
	m_pDeviceContext->VSSetConstantBuffers(4, 1, &buffer);

	// �ȼ����̴� ����
	m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	// �ȼ����̴� �������
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pCBTransformW);
	m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pCBTransformVP);
	m_pDeviceContext->PSSetConstantBuffers(2, 1, &m_pCBDirectionLight);
	m_pDeviceContext->PSSetConstantBuffers(3, 1, &m_pGpuCbMaterial);

	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerLinear);

	//����Ʈ ������Ʈ	
	m_Light.Direction.Normalize();
	m_pDeviceContext->UpdateSubresource(m_pCBDirectionLight, 0, nullptr, &m_Light, 0, 0);

	// ���������� ������Ʈ
	m_TransformVP.mView = m_View.Transpose();
	m_TransformVP.mProjection = m_Projection.Transpose();
	m_pDeviceContext->UpdateSubresource(m_pCBTransformVP, 0, nullptr, &m_TransformVP, 0, 0);

	RenderSkeletalMeshInstance();
	RenderStaticMeshInstance();
	RenderDebugDraw();
	RenderImGui();

	m_pSwapChain->Present(0, 0);	// Present our back buffer to our front buffer
}

void D3DRenderManager::RenderDebugDraw()
{
	m_pDeviceContext->OMSetBlendState( DebugDraw::g_States->Opaque(), nullptr, 0xFFFFFFFF);	
	//m_pDeviceContext->RSSetState(DebugDraw::g_States->CullNone());

	DebugDraw::g_BatchEffect->Apply(m_pDeviceContext);
	DebugDraw::g_BatchEffect->SetView(m_View);
	DebugDraw::g_BatchEffect->SetProjection(m_Projection);

	m_pDeviceContext->IASetInputLayout(DebugDraw::g_pBatchInputLayout.Get());

	DebugDraw::g_Batch->Begin();

	for (auto& SkeletalMeshComponent : m_SkeletalMeshComponents)
	{
		DebugDraw::Draw(DebugDraw::g_Batch.get(), SkeletalMeshComponent->m_BoundingBox, Colors::Blue); // BoundingBox
	}
	for (auto& StaticMeshComponent : m_StaticMeshComponents)
	{
		DebugDraw::Draw(DebugDraw::g_Batch.get(), StaticMeshComponent->m_BoundingBox, Colors::Blue); // BoundingBox
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
		ImGui::Begin("Properties");


		ImGui::Text("Frame average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		std::string str;
		GetVideoMemoryInfo(str);
		ImGui::Text("VideoMemory: %s", str.c_str());
		GetSystemMemoryInfo(str);
		ImGui::Text("SystemMemory: %s", str.c_str());

		ImGui::Text("Light");
		ImGui::SliderFloat3("LightDirection", (float*)&m_Light.Direction, -1.0f, 1.0f);
		ImGui::ColorEdit3("LightRadiance", (float*)&m_Light.Radiance);

		ImGui::Text("BackBuffer");
		ImGui::ColorEdit3("clear color", (float*)&m_ClearColor); // Edit 3 floats representing a color	

		AddDebugVector3ToImGuiWindow("EyePosition", m_Light.EyePosition);
		AddDebugVector3ToImGuiWindow("LookAt", m_LookAt);
		AddDebugMatrixToImGuiWindow("ViewMatrix", m_View);


		if (m_pImGuiRender)
			m_pImGuiRender->ImGuiRender();

		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


void D3DRenderManager::RenderSkeletalMeshInstance()
{
	m_pDeviceContext->IASetInputLayout(m_pSkeletalMeshInputLayout);
	m_pDeviceContext->VSSetShader(m_pSkeletalMeshVertexShader, nullptr, 0);

	m_SkeletalMeshInstance.sort([](const SkeletalMeshInstance* lhs, const SkeletalMeshInstance* rhs)
		{
			return lhs->m_pMaterial < rhs->m_pMaterial;
		});

	Material* pPrevMaterial = nullptr;
	for (const auto& meshInstance : m_SkeletalMeshInstance)
	{
		if (pPrevMaterial != meshInstance->m_pMaterial)
		{
			ApplyMaterial(meshInstance->m_pMaterial);	// ���͸��� ����
			pPrevMaterial = meshInstance->m_pMaterial;
		}
		// ����ȷ�Ʈ ������Ʈ						
		meshInstance->UpdateMatrixPallete(&m_MatrixPalette);
		m_cbMatrixPallete.SetData(m_pDeviceContext, m_MatrixPalette);

		// Draw
		meshInstance->Render(m_pDeviceContext);
	}
	m_SkeletalMeshInstance.clear();
}

void D3DRenderManager::RenderStaticMeshInstance()
{
	m_pDeviceContext->IASetInputLayout(m_pStaticMeshInputLayout);
	m_pDeviceContext->VSSetShader(m_pStaticMeshVertexShader, nullptr, 0);

	m_StaticMeshInstance.sort([](const StaticMeshInstance* lhs, const StaticMeshInstance* rhs)
		{
			return lhs->m_pMaterial < rhs->m_pMaterial;
		});

	Material* pPrevMaterial = nullptr;
	for (const auto& meshInstance : m_StaticMeshInstance)
	{
		if (pPrevMaterial != meshInstance->m_pMaterial)
		{
			ApplyMaterial(meshInstance->m_pMaterial);	// ���͸��� ����
			pPrevMaterial = meshInstance->m_pMaterial;
		}

		m_TransformW.mWorld = meshInstance->m_pNodeWorldTransform->Transpose();
		m_pDeviceContext->UpdateSubresource(m_pCBTransformW, 0, nullptr, &m_TransformW, 0, 0);

		// Draw
		meshInstance->Render(m_pDeviceContext);
	}
	m_StaticMeshInstance.clear();
}

void D3DRenderManager::CreateSkeletalMesh_VS_IL()
{
	HRESULT hr;
	// 2. Render() ���� ���������ο� ���ε��� InputLayout ���� 	
	D3D_SHADER_MACRO defines[] =
	{
		{"VERTEX_SKINNING",""}, // ��ũ�� �̸��� ���� ����
		{nullptr, nullptr}    // �迭�� ���� ��Ÿ���� ���� nullptr�� ������.
	};
	ID3D10Blob* vertexShaderBuffer = nullptr;
	hr = CompileShaderFromFile(L"07_VertexShader.hlsl", defines, "main", "vs_5_0", &vertexShaderBuffer);
	if (FAILED(hr))
	{
		hr = D3DReadFileToBlob(L"07_VertexShader.cso", &vertexShaderBuffer);
	}
	HR_T(hr);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES" , 0, DXGI_FORMAT_R32G32B32A32_SINT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHTS" , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pSkeletalMeshInputLayout);

	// 3. Render() ���� ���������ο� ���ε���  ���ؽ� ���̴� ����
	HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &m_pSkeletalMeshVertexShader));
	SAFE_RELEASE(vertexShaderBuffer);
}

void D3DRenderManager::CreateStaticMesh_VS_IL()
{
	HRESULT hr;
	// 2. Render() ���� ���������ο� ���ε��� InputLayout ���� 	
	D3D_SHADER_MACRO defines[] =
	{
		{"",""}, // ��ũ�� �̸��� ���� ����
		{nullptr, nullptr}    // �迭�� ���� ��Ÿ���� ���� nullptr�� ������.
	};
	ID3D10Blob* vertexShaderBuffer = nullptr;
	hr = CompileShaderFromFile(L"07_VertexShader.hlsl", nullptr, "main", "vs_5_0", &vertexShaderBuffer);
	if (FAILED(hr))
	{
		hr = D3DReadFileToBlob(L"07_VertexShader.cso", &vertexShaderBuffer);
	}
	HR_T(hr);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },	
		{ "BITANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pStaticMeshInputLayout);

	// 3. Render() ���� ���������ο� ���ε���  ���ؽ� ���̴� ����
	HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &m_pStaticMeshVertexShader));
	SAFE_RELEASE(vertexShaderBuffer);
}

void D3DRenderManager::CreatePS()
{
	HRESULT hr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	hr = CompileShaderFromFile(L"07_PixelShader.hlsl", nullptr, "main", "ps_5_0", &pixelShaderBuffer);
	if (FAILED(hr))
	{
		hr = D3DReadFileToBlob(L"07_PixelShader.cso", &pixelShaderBuffer);
	}
	HR_T(hr);

	HR_T(m_pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader));
	SAFE_RELEASE(pixelShaderBuffer);
}

void D3DRenderManager::ApplyMaterial(Material* pMaterial)
{
	if(pMaterial->m_pBaseColor)
		m_pDeviceContext->PSSetShaderResources(0, 1, pMaterial->m_pBaseColor->m_pTextureSRV.GetAddressOf());	

	if (pMaterial->m_pNormal)
		m_pDeviceContext->PSSetShaderResources(1, 1, pMaterial->m_pNormal->m_pTextureSRV.GetAddressOf());

	if (pMaterial->m_pSpecular)
		m_pDeviceContext->PSSetShaderResources(2, 1, pMaterial->m_pSpecular->m_pTextureSRV.GetAddressOf());

	if (pMaterial->m_pEmissive)
		m_pDeviceContext->PSSetShaderResources(3, 1, pMaterial->m_pEmissive->m_pTextureSRV.GetAddressOf());

	if (pMaterial->m_pOpacity)
		m_pDeviceContext->PSSetShaderResources(4, 1, pMaterial->m_pOpacity->m_pTextureSRV.GetAddressOf());

	if (pMaterial->m_pMetalness)
		m_pDeviceContext->PSSetShaderResources(5, 1, pMaterial->m_pMetalness->m_pTextureSRV.GetAddressOf());

	if (pMaterial->m_pRoughness)
		m_pDeviceContext->PSSetShaderResources(6, 1, pMaterial->m_pRoughness->m_pTextureSRV.GetAddressOf());

		
	m_CpuCbMaterial.UseBaseColorMap = pMaterial->m_pBaseColor != nullptr ? true : false;
	m_CpuCbMaterial.UseNormalMap = pMaterial->m_pNormal != nullptr ? true : false;
	m_CpuCbMaterial.UseSpecularMap = pMaterial->m_pSpecular != nullptr ? true : false;
	m_CpuCbMaterial.UseEmissiveMap = pMaterial->m_pEmissive != nullptr ? true : false;
	m_CpuCbMaterial.UseOpacityMap = pMaterial->m_pOpacity != nullptr ? true : false;
	m_CpuCbMaterial.UseMetalnessMap = pMaterial->m_pMetalness != nullptr ? true : false;
	m_CpuCbMaterial.UseRoughnessMap = pMaterial->m_pRoughness != nullptr ? true : false;

	if (m_CpuCbMaterial.UseOpacityMap && m_pAlphaBlendState != nullptr)
		m_pDeviceContext->OMSetBlendState(m_pAlphaBlendState, nullptr, 0xffffffff); // ���ĺ��� ���¼��� , �ٸ��ɼ��� �⺻�� 
	else
		m_pDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);	// �������� , �ٸ��ɼ��� �⺻��

	m_pDeviceContext->UpdateSubresource(m_pGpuCbMaterial, 0, nullptr, &m_CpuCbMaterial, 0, 0);
}

void  D3DRenderManager::GetVideoMemoryInfo(std::string& out)
{
	DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
	m_pDXGIAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo);

	out = std::to_string(videoMemoryInfo.CurrentUsage / 1024 / 1024) + " MB" + "/" + std::to_string(videoMemoryInfo.Budget / 1024 / 1024) + " MB";
}

void D3DRenderManager::GetSystemMemoryInfo(std::string& out)
{
	HANDLE hProcess = GetCurrentProcess();	
	PROCESS_MEMORY_COUNTERS_EX pmc;
	pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);
	GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	out = std::to_string( (pmc.PagefileUsage) / 1024 / 1024) + " MB";	
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

