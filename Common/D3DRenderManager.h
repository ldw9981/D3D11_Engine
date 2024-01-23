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
	Matrix mShadowView;				   // 그림자를 렌더링할 월드의 역행렬
	Matrix mShadowProjection;		   // 그림자를 렌더링할 투영방식 직교투영 행렬
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
	float AmbientOcclusion = 0.35f;				// 4
	int UseGammaCorrection = true;				// 4  
	float Gamma = 1.6f;							// 4
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
class EnvironmentActor;
class EnvironmentMeshComponent;

class D3DRenderManager
{
public:
	D3DRenderManager();
	~D3DRenderManager();

	static D3DRenderManager* Instance;
	ComPtr<IDXGIFactory4> m_pDXGIFactory;		// DXGI팩토리
	ComPtr<IDXGIAdapter3> m_pDXGIAdapter;		// 비디오카드 정보에 접근 가능한 인터페이스
	// 렌더링 파이프라인을 구성하는 필수 객체의 인터페이스
	static ID3D11Device* m_pDevice;						// 디바이스
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;		// 즉시 디바이스 컨텍스트
	ComPtr<IDXGISwapChain> m_pSwapChain;					// 스왑체인
	ComPtr<ID3D11RenderTargetView> m_pBackBufferRTV;	// 렌더링 타겟뷰
	ComPtr<ID3D11DepthStencilView> m_pDefaultDSV;	// 깊이/스텐실 뷰

	ComPtr<ID3D11Texture2D>				m_pShadowMap;
	ComPtr<ID3D11RenderTargetView>		m_pShadowMapRTV;
	ComPtr<ID3D11ShaderResourceView>	m_pShadowMapSRV;
	ComPtr<ID3D11DepthStencilView>		m_pShadowMapDSV;

	// 렌더링 파이프라인에 적용하는 리소스 객체의 인터페이스
	ShaderProgram m_ShaderBaseSkeletalMesh;
	ShaderProgram m_ShaderBaseStaticMesh;
	ShaderProgram m_ShaderShadowSkeletalMesh;
	ShaderProgram m_ShaderShadowStaticMesh;
	ShaderProgram m_ShaderEnvironment;

	ComPtr<ID3D11SamplerState> m_pSamplerLinear;		// 샘플러 상태.
	ComPtr<ID3D11SamplerState> m_pSamplerClamp;		// 샘플러 상태.

	ComPtr<ID3D11BlendState> m_pAlphaBlendState;		// 블렌드 상태 변경 (반투명처리를위한 블렌드상태)
	ComPtr<ID3D11RasterizerState> m_pRasterizerStateCCW; // 컬 모드 CCW
	ComPtr<ID3D11RasterizerState> m_pRasterizerStateCW; // 컬 모드 CW


	ComPtr<ID3D11Buffer> m_pCBMaterial;				// 상수 버퍼: 변환행렬
	ComPtr<ID3D11Buffer> m_pCBTransformW;				// 상수 버퍼: 변환행렬
	ComPtr<ID3D11Buffer> m_pCBTransformVP;				// 상수 버퍼: 변환행렬
	ComPtr<ID3D11Buffer> m_pCBDirectionLight;		// 상수 버퍼: 방향광
	ComPtr<ID3D11Buffer> m_pCBGlobal;				 // 기타 여러가지

	ConstantBuffer<CB_MatrixPalette> m_cbMatrixPallete;			
	ConstantBuffer<CB_MarterialOverride> m_cbMaterialOverride;
	FrameBuffer	m_ShadowBuffer;

	// 렌더링 파이프라인에 적용하는 정보
	Matrix  m_View;					// 뷰좌표계 공간으로 변환을 위한 행렬.
	Matrix  m_Projection;			// 단위장치좌표계( Normalized Device Coordinate) 공간으로 변환을 위한 행렬.
	Matrix  m_ShadowView;
	Matrix  m_ShadowProjection;
	Vector2 m_ShadowProjectionNearFar = {7000.0f, 20000.0f};
	Vector2 m_CameraProjectionNearFar = {1.0f, 100000.0f };
	float m_ShadowUpDistFromLootAt = 13000;					// 카메라 기준으로 떨어지는 거리
	float m_ShadowForwardDistFromCamera = 3500;			// 카메라 기준으로 떨어지는 거리
	Math::Vector3 m_ShadowLootAt = { 0.0f, 0.0f, 0.0f };	// 카메라 위치기준으로 계산됨
	Math::Vector3 m_ShadowPos = { 0.0f, 0.0f, 0.0f };		// 카메라 위치기준으로 계산됨
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
	std::list<StaticMeshInstance*>		m_StaticMeshInstanceOpaque;			// 불투명 
	std::list<StaticMeshInstance*>		m_StaticMeshInstanceTranslucent;	// 반투명 

	std::list<SkeletalMeshInstance*>	m_SkeletalMeshInstanceOpaque;			// 불투명 
	std::list<SkeletalMeshInstance*>	m_SkeletalMeshInstanceTranslucent;		// 반투명 


	std::list<SkeletalMeshComponent*>	m_SkeletalMeshComponents;
	std::list<StaticMeshComponent*>		m_StaticMeshComponents;

	std::list<IImGuiRenderable*>		m_ImGuiRenders;

	std::list<std::function<void()>>	m_ImGuiRenderFuncs;

	std::list<DebugRay>					m_DebugDrawLines;
	std::weak_ptr<CameraComponent>		m_pCamera;
	EnvironmentMeshComponent* m_pEnvironmentMeshComponent = nullptr;

	BoundingFrustum m_FrustumCamera;
	BoundingFrustum m_FrustumShadow;
	
	int m_nDrawComponentCount = 0;
	bool m_bFreezeCulling = false;
	bool m_bFreezeShadow = false;
	bool m_bDrawShadowFrustum = false;
	bool m_bDrawDebugCulling = false;
	bool m_bDrawDebugCollision = false;
	bool m_bUseMSAA = false;
	bool m_bDebugShadow = false;
	bool m_bUseCulling = true;
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
	void SetEnvironment(EnvironmentActor* pActor);
	void SetDirectionLight(Math::Vector3 direction);
	void SetMaterialOverride(bool bOverride) { m_MaterialOverride.UseMarterialOverride = bOverride; }

	bool GetFreezeCulling() const { return m_bFreezeCulling; }
	void SetFreezeCulling(bool val) { m_bFreezeCulling = val; }
	void CreateMousePickingRay(float ScreenX,float ScreenY,Math::Vector3 & Origin, Math::Vector3& Direction);
	
	//컴포넌트 등록하기
	void AddSkeletalMeshComponent(SkeletalMeshComponent* pSkeletalMeshComponent);
	void RemoveSkeletalMeshComponent(SkeletalMeshComponent* pSkeletalMeshComponent);
	void AddStaticMeshComponent(StaticMeshComponent* pStaticMeshComponent);
	void RemoveStaticMeshComponent(StaticMeshComponent* pStaticMeshComponent);

	// ImGui 디버깅 정보 텍스트 그리기
	void AddImguiRenderable(IImGuiRenderable* pIImGuiRenderable);
	void RemoveImguiRenderable(IImGuiRenderable* pIImGuiRenderable);

	void AddImGuiRenderFunc(std::function<void()> func);
	void RemoveImGuiRenderFunc(std::function<void()> func);


	void AddDebugStringToImGuiWindow(const std::string& header, const std::string& str);
	void AddDebugMatrixToImGuiWindow(const std::string& header, const Matrix& mat);
	void AddDebugVector4ToImGuiWindow(const std::string& header, const Vector4& value);
	void AddDebugVector3ToImGuiWindow(const std::string& header, const Vector3& value);
	void AddDebugVector2ToImGuiWindow(const std::string& header, const Vector2& value);
	void AddDebugFloatToImGuiWindow(const std::string& header, const float& value);
	
	//Geometry 디버깅 정보  그리기
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
