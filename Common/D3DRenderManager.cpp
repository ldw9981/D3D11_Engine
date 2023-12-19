#include "pch.h"
#include "D3DRenderManager.h"
#include "Model.h"
#include "Helper.h"
#include <d3dcompiler.h>
#include <Directxtk/DDSTextureLoader.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <winerror.h>
#include <dxgidebug.h>


#include "Animation.h"


#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")



D3DRenderManager* D3DRenderManager::Instance = nullptr;

ID3D11Device* D3DRenderManager::m_pDevice = nullptr;

D3DRenderManager::D3DRenderManager()
{
	Instance = this;
}

D3DRenderManager::~D3DRenderManager()
{
	Uninitialize();
}




void D3DRenderManager::AddModel(SkeletalMeshModel* pModel)
{
	m_Models.push_back(pModel);
}

void D3DRenderManager::AddModel(StaticMeshModel* pModel)
{
	m_StaticModels.push_back(pModel);
}

bool D3DRenderManager::Initialize(HWND Handle,UINT Width, UINT Height)
{
	m_hWnd = Handle;
	HRESULT hr = 0;	// �����.

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

	// 1. Render() ���� ���������ο� ���ε��� ���ؽ� ���۹� ���� ���� �غ�	

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
		{ "BLENDINDICES" , 0, DXGI_FORMAT_R32G32B32A32_SINT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHTS" , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pSkinningInputLayout);

	// 3. Render() ���� ���������ο� ���ε���  ���ؽ� ���̴� ����
	HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &m_pSkinningVertexShader));
	SAFE_RELEASE(vertexShaderBuffer);


	// 5. Render() ���� ���������ο� ���ε��� �ȼ� ���̴� ����
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


	// 6. Render() ���� ���������ο� ���ε��� ��� ���� ����
	// Create the constant buffer
	D3D11_BUFFER_DESC bd = {};
	bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CB_Transform);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, &m_pCBTransform));

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

	return true;
}

void D3DRenderManager::Uninitialize()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//	SAFE_RELEASE(m_cbMatrixPallete.GetBuffer()); //ComPtr
	SAFE_RELEASE(m_pCBDirectionLight);
	SAFE_RELEASE(m_pCBTransform);
	SAFE_RELEASE(m_pGpuCbMaterial);
	SAFE_RELEASE(m_pAlphaBlendState);
	SAFE_RELEASE(m_pSamplerLinear);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pSkinningInputLayout);
	SAFE_RELEASE(m_pSkinningVertexShader);

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
	m_World = Matrix::CreateScale(m_MeshScale) * Matrix::CreateFromYawPitchRoll(Vector3(XMConvertToRadians(m_Rotation.x), XMConvertToRadians(m_Rotation.y), 0));

	m_View = XMMatrixLookToLH(m_CameraPos, Vector3(0, 0, 1), Vector3(0, 1, 0));

	m_Light.EyePosition = m_CameraPos;
}

void D3DRenderManager::Render()
{
	// Clear the back buffer
	const float clear_color_with_alpha[4] = { m_ClearColor.x , m_ClearColor.y , m_ClearColor.z, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, clear_color_with_alpha);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->IASetInputLayout(m_pSkinningInputLayout);


	m_pDeviceContext->VSSetShader(m_pSkinningVertexShader, nullptr, 0);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pCBTransform);
	m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pCBDirectionLight);

	//m_pDeviceContext->VSSetConstantBuffers(3, 1, &m_pCBMatrixPalette);
	auto buffer = m_cbMatrixPallete.GetBuffer();
	m_pDeviceContext->VSSetConstantBuffers(3, 1, &buffer);

	m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pCBTransform);
	m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pCBDirectionLight);
	m_pDeviceContext->PSSetConstantBuffers(2, 1, &m_pGpuCbMaterial);

	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerLinear);

	m_Light.Direction.Normalize();
	m_pDeviceContext->UpdateSubresource(m_pCBDirectionLight, 0, nullptr, &m_Light, 0, 0);


	for (const auto& ModelPtr : m_Models) 
	{
		for (size_t i = 0; i < ModelPtr->m_MeshInstances.size(); i++)
		{
			SkeletalMeshInstance& meshInstance = ModelPtr->m_MeshInstances[i];

			// ���͸��� ����
			assert(meshInstance.m_pMaterial!=nullptr);
			ApplyMaterial(meshInstance.m_pMaterial);			

			// ���̷�Ż �޽�(����������) ����ȷ�Ʈ ������Ʈ						
			meshInstance.UpdateMatrixPallete(&m_MatrixPalette, &ModelPtr->m_SceneResource->m_Skeleton);
			m_cbMatrixPallete.SetData(m_pDeviceContext, m_MatrixPalette);
			
			/*
			else
			{
				// MVP Matrix ����
				m_Transform.mWorld = mesh.m_pNodeWorldTransform->Transpose();
			}
			*/

			m_Transform.mView = m_View.Transpose();
			m_Transform.mProjection = m_Projection.Transpose();
			m_pDeviceContext->UpdateSubresource(m_pCBTransform, 0, nullptr, &m_Transform, 0, 0);

			// Draw
			meshInstance.Render(m_pDeviceContext);
		}
	}
	m_Models.clear();

	for (const auto& ModelPtr : m_StaticModels)
	{
		for (size_t i = 0; i < ModelPtr->m_MeshInstances.size(); i++)
		{
			StaticMeshInstance& meshInstance = ModelPtr->m_MeshInstances[i];

			// ���͸��� ����
			assert(meshInstance.m_pMaterial != nullptr);
			ApplyMaterial(meshInstance.m_pMaterial);			
				
			m_Transform.mWorld = meshInstance.m_pNodeWorldTransform->Transpose();
			m_Transform.mView = m_View.Transpose();
			m_Transform.mProjection = m_Projection.Transpose();
			m_pDeviceContext->UpdateSubresource(m_pCBTransform, 0, nullptr, &m_Transform, 0, 0);

			// Draw
			meshInstance.Render(m_pDeviceContext);
		}
	}
	m_StaticModels.clear();


	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	{
		ImGui::Begin("Properties");

		ImGui::Text("Cube");
		ImGui::SliderFloat("Scale", (float*)&m_MeshScale, 1, 100);
		ImGui::SliderFloat2("Rotation", (float*)&m_Rotation, -180, 180);

		ImGui::Text("Light");
		ImGui::SliderFloat3("LightDirection", (float*)&m_Light.Direction, -1.0f, 1.0f);
		ImGui::ColorEdit4("LightAmbient", (float*)&m_Light.Ambient);
		ImGui::ColorEdit4("LightDiffuse", (float*)&m_Light.Diffuse);
		ImGui::ColorEdit4("LightSpecular", (float*)&m_Light.Specular);

		ImGui::Text("Material");
		ImGui::ColorEdit4("MaterialAmbient", (float*)&m_CpuCbMaterial.Ambient);
		ImGui::ColorEdit4("MaterialDiffuse", (float*)&m_CpuCbMaterial.Diffuse);
		ImGui::ColorEdit4("MaterialSpecular", (float*)&m_CpuCbMaterial.Specular);
		ImGui::ColorEdit4("MaterialEmissive", (float*)&m_CpuCbMaterial.Emissive);
		ImGui::SliderFloat("MaterialSpecularPower", (float*)&m_CpuCbMaterial.SpecularPower, 2.0f, 4096.0f);

		ImGui::Text("Camera");
		ImGui::SliderFloat3("Position", (float*)&m_CameraPos, -2000.0f, 2000.0f);

		ImGui::Text("BackBuffer");
		ImGui::ColorEdit4("clear color", (float*)&m_ClearColor); // Edit 3 floats representing a color	
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	m_pSwapChain->Present(0, 0);	// Present our back buffer to our front buffer
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

	m_CpuCbMaterial.Diffuse = pMaterial->m_Color;
	m_CpuCbMaterial.UseDiffuseMap = pMaterial->m_pBaseColor != nullptr ? true : false;
	m_CpuCbMaterial.UseNormalMap = pMaterial->m_pNormal != nullptr ? true : false;
	m_CpuCbMaterial.UseSpecularMap = pMaterial->m_pSpecular != nullptr ? true : false;
	m_CpuCbMaterial.UseEmissiveMap = pMaterial->m_pEmissive != nullptr ? true : false;
	m_CpuCbMaterial.UseOpacityMap = pMaterial->m_pOpacity != nullptr ? true : false;

	if (m_CpuCbMaterial.UseOpacityMap && m_pAlphaBlendState != nullptr)
		m_pDeviceContext->OMSetBlendState(m_pAlphaBlendState, nullptr, 0xffffffff); // ���ĺ��� ���¼��� , �ٸ��ɼ��� �⺻�� 
	else
		m_pDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);	// �������� , �ٸ��ɼ��� �⺻��

	m_pDeviceContext->UpdateSubresource(m_pGpuCbMaterial, 0, nullptr, &m_CpuCbMaterial, 0, 0);
}



