#pragma once
#include <dxgi1_4.h>
#include <DirectXtk/BufferHelpers.h>
#include <DirectXCollision.h>
#include "FrameBuffer.h"
#include "ShaderProgram.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;
using namespace Microsoft::WRL;

struct CB_TransformW
{
	Matrix mWorld;
};

struct CB_TransformVP
{
	Matrix mView;
	Matrix mProjection;
	Matrix mShadowView;				   // �׸��ڸ� �������� ������ �����
	Matrix mShadowProjection;		   // �׸��ڸ� �������� ������� �������� ���
};

struct CB_DirectionLight
{
	Vector3 Direction = { 0.0f, -0.7f, -0.7f };
	float pad0 = 0.0f;
	Vector3 Radiance = { 1.0f,1.0f,1.0f };
	float pad1 = 0.0f;
	Vector3 EyePosition = {};
	float pad2 = 0.0f;
};

struct CB_MatrixPalette
{
	Math::Matrix Array[128];
};

struct CB_Marterial
{
	int UseBaseColorMap = true;					// 4
	int UseNormalMap = true;					// 4
	int UseSpecularMap = true;                  // 4 		
	int UseEmissiveMap = true;					// 4 16byte	
	int UseOpacityMap = true;					// 4
	int UseMetalnessMap = true;					// 4
	int UseRoughnessMap = true;					// 4	
	int pad = 0;									// 4 16	
};

struct CB_Global
{
	int UseIBL = true;							// 4  
	float AmbientOcclusion = 1.0f;				// 4
	int UseGammaCorrection = true;				// 4  
	float Gamma = 2.2f;							// 4
};


struct CB_MarterialOverride
{
	int UseMarterialOverride = false;								// 4 
	Vector3 BaseColorOverride = Vector3(1.0f, 1.0f, 1.0f);	// 12   16	
	float MetalnessOverride = 0.0f;					// 4
	float RoughnessOverride = 0.0f;					// 4		
	Vector2 pad2;					// 8   16byte	
};



class IImGuiRenderable
{
public:
	virtual void ImGuiRender() = 0;
};

struct DebugRay
{
	Math::Vector3 origin;
	Math::Vector3 direction;
	bool normalize;
	Math::Vector3 color;
	float time;
};


class Material;
class StaticMeshInstance;
class SkeletalMeshInstance;
class StaticMeshComponent;
class SkeletalMeshComponent;
class CameraComponent;
class CollisionComponent;
class EnvironmentMeshComponent;

class D3DRenderManager
{
public:
	D3DRenderManager();
	~D3DRenderManager();

	static D3DRenderManager* Instance;
	ComPtr<IDXGIFactory4> m_pDXGIFactory;		// DXGI���丮
	ComPtr<IDXGIAdapter3> m_pDXGIAdapter;		// ����ī�� ������ ���� ������ �������̽�
	// ������ ������������ �����ϴ� �ʼ� ��ü�� �������̽�
	static ID3D11Device* m_pDevice;						// ����̽�
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;		// ��� ����̽� ���ؽ�Ʈ
	ComPtr<IDXGISwapChain> m_pSwapChain;					// ����ü��
	ComPtr<ID3D11RenderTargetView> m_pBackBufferRTV;	// ������ Ÿ�ٺ�
	ComPtr<ID3D11DepthStencilView> m_pDefaultDSV;	// ����/���ٽ� ��

	ComPtr<ID3D11Texture2D>				m_pShadowMap;
	ComPtr<ID3D11RenderTargetView>		m_pShadowMapRTV;
	ComPtr<ID3D11ShaderResourceView>	m_pShadowMapSRV;
	ComPtr<ID3D11DepthStencilView>		m_pShadowMapDSV;

	// ������ ���������ο� �����ϴ� ���ҽ� ��ü�� �������̽�
	ShaderProgram m_ShaderBaseSkeletalMesh;
	ShaderProgram m_ShaderBaseStaticMesh;
	ShaderProgram m_ShaderShadowSkeletalMesh;
	ShaderProgram m_ShaderShadowStaticMesh;
	ShaderProgram m_ShaderEnvironment;

	ComPtr<ID3D11SamplerState> m_pSamplerLinear;		// ���÷� ����.
	ComPtr<ID3D11SamplerState> m_pSamplerClamp;		// ���÷� ����.

	ComPtr<ID3D11BlendState> m_pAlphaBlendState;		// ���� ���� ���� (������ó�������� �������)
	ComPtr<ID3D11RasterizerState> m_pRasterizerStateCCW; // �� ��� CCW
	ComPtr<ID3D11RasterizerState> m_pRasterizerStateCW; // �� ��� CW


	ComPtr<ID3D11Buffer> m_pCBMaterial;				// ��� ����: ��ȯ���
	ComPtr<ID3D11Buffer> m_pCBTransformW;				// ��� ����: ��ȯ���
	ComPtr<ID3D11Buffer> m_pCBTransformVP;				// ��� ����: ��ȯ���
	ComPtr<ID3D11Buffer> m_pCBDirectionLight;		// ��� ����: ���Ɽ
	ComPtr<ID3D11Buffer> m_pCBGlobal;				 // ��Ÿ ��������

	ConstantBuffer<CB_MatrixPalette> m_cbMatrixPallete;			
	ConstantBuffer<CB_MarterialOverride> m_cbMaterialOverride;
	FrameBuffer	m_ShadowBuffer;

	// ������ ���������ο� �����ϴ� ����
	Matrix  m_View;					// ����ǥ�� �������� ��ȯ�� ���� ���.
	Matrix  m_Projection;			// ������ġ��ǥ��( Normalized Device Coordinate) �������� ��ȯ�� ���� ���.
	Matrix  m_ShadowView;
	Matrix  m_ShadowProjection;
	Vector2 m_ShadowProjectionNearFar = {1000.0f, 10000.0f};
	Math::Vector3 m_ShadowLootAt = { 0.0f, 0.0f, 0.0f };
	Math::Vector3 m_ShadowPos = { 0.0f, 1000.0f, 0.0f };
	Vector3	m_LookAt = { 0.0f, 0.0f, 0.0f };
	Vector3 m_ShadowDir = { 0.0f, 0.0f, 0.0f };
	Vector3 m_ClearColor = { 0.0f, 0.0f, 0.0f };
	CB_TransformW m_TransformW;
	CB_TransformVP m_TransformVP;
	CB_Marterial m_CpuCbMaterial;
	CB_DirectionLight m_Light;
	CB_Global m_Global;

	CB_MatrixPalette m_MatrixPalette;
	CB_MarterialOverride m_MaterialOverride;
	D3D11_VIEWPORT m_BaseViewport;
	D3D11_VIEWPORT m_ShadowViewport;
	HWND m_hWnd = nullptr;
	std::list<StaticMeshInstance*>		m_StaticMeshInstanceOpaque;			// ������ 
	std::list<StaticMeshInstance*>		m_StaticMeshInstanceTranslucent;	// ������ 

	std::list<SkeletalMeshInstance*>	m_SkeletalMeshInstanceOpaque;			// ������ 
	std::list<SkeletalMeshInstance*>	m_SkeletalMeshInstanceTranslucent;		// ������ 


	std::list<SkeletalMeshComponent*>	m_SkeletalMeshComponents;
	std::list<StaticMeshComponent*>		m_StaticMeshComponents;

	std::list<IImGuiRenderable*>		m_ImGuiRenders;
	std::list<DebugRay>					m_DebugDrawLines;
	std::weak_ptr<CameraComponent>		m_pCamera;
	std::weak_ptr<EnvironmentMeshComponent> m_pEnvironmentMeshComponent;

	BoundingFrustum m_FrustumCamera;
	BoundingFrustum m_FrustumShadow;
	float m_ShadowDistFromCamera = 3000;
	int m_nDrawComponentCount = 0;
	bool m_bFreezeCulling = false;
	bool m_bFreezeShadow = false;
	bool m_bDrawDebugShadow = false;
	bool m_bDrawDebugCulling = false;
	bool m_bDrawDebugCollision = false;
	bool m_bUseMSAA = false;
	UINT m_SampleQuality = 0;
	UINT m_SampleCount = 4;
public:
	bool Initialize(HWND Handle,UINT Width, UINT Height);
	void Uninitialize();
	void Update(float DeltaTime);
	void Render();
	void ApplyMaterial(Material* pMaterial);
	void GetVideoMemoryInfo(std::string& out);
	void GetSystemMemoryInfo(std::string& out);

	std::weak_ptr<CameraComponent> GetCamera() const { return m_pCamera; }
	void SetCamera(std::weak_ptr<CameraComponent> val) { m_pCamera = val; }
	std::weak_ptr<EnvironmentMeshComponent> GetEnvironmentMeshComponent() const;
	void SetEnvironment(std::weak_ptr<EnvironmentMeshComponent> val);

	bool GetFreezeCulling() const { return m_bFreezeCulling; }
	void SetFreezeCulling(bool val) { m_bFreezeCulling = val; }
	void CreateMousePickingRay(float ScreenX,float ScreenY,Math::Vector3 & Origin, Math::Vector3& Direction);

	//������Ʈ ����ϱ�
	void AddSkeletalMeshComponent(SkeletalMeshComponent* pSkeletalMeshComponent);
	void RemoveSkeletalMeshComponent(SkeletalMeshComponent* pSkeletalMeshComponent);
	void AddStaticMeshComponent(StaticMeshComponent* pStaticMeshComponent);
	void RemoveStaticMeshComponent(StaticMeshComponent* pStaticMeshComponent);

	// ImGui ����� ���� �ؽ�Ʈ �׸���
	void AddImguiRenderable(IImGuiRenderable* pIImGuiRenderable);
	void RemoveImguiRenderable(IImGuiRenderable* pIImGuiRenderable);
	void AddDebugStringToImGuiWindow(const std::string& header, const std::string& str);
	void AddDebugMatrixToImGuiWindow(const std::string& header, const Matrix& mat);
	void AddDebugVector4ToImGuiWindow(const std::string& header, const Vector4& value);
	void AddDebugVector3ToImGuiWindow(const std::string& header, const Vector3& value);
	void AddDebugVector2ToImGuiWindow(const std::string& header, const Vector2& value);
	void AddDebugFloatToImGuiWindow(const std::string& header, const float& value);
	
	//Geometry ����� ����  �׸���
	void AddDebugDrawLine(const Math::Vector3& origin, const Math::Vector3& direction, bool normalize, const Math::Vector3& color, float time);

	ComPtr<ID3D11SamplerState> CreateSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode) const;

	HRESULT CreateTextureFromFile(const wchar_t* szFileName, ID3D11ShaderResourceView** textureView, ID3D11Resource** texture = nullptr);
	HRESULT CreateSamplerStateE(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, ID3D11SamplerState** ppSamplerState);
	FrameBuffer CreateFrameBuffer(UINT width, UINT height, UINT samples, DXGI_FORMAT renderTargetFormat, DXGI_FORMAT depthstencilFormat);
	void SetBaseViewPort(UINT Width, UINT Height);
	void SetShadowViewPort(UINT Width, UINT Height);
private:
	void CreateBuffers();
	void CreateShaders();	
	void CreateConstantBuffer();
	void CreateSamplerState();
	void CreateRasterizerState();
	void CreateBlendState();
	void CreateEnvironment();
	

	void AddMeshInstance(SkeletalMeshComponent* pModel);
	void AddMeshInstance(StaticMeshComponent* pModel);
	void SortMeshInstance();
	void ClearMeshInstance();

	void RenderDebugDraw();
	void RenderImGui();
	void RenderSkeletalMeshInstanceOpaque();
	void RenderStaticMeshInstanceOpaque();
	void RenderSkeletalMeshInstanceTranslucent();
	void RenderStaticMeshInstanceTranslucent();

	void RenderEnvironment();
	void RenderShadowDepth();
	void MSAACheck(DXGI_FORMAT format, UINT& SampleCount, UINT& Quality);
};
