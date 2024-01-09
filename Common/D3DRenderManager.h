#pragma once
#include <dxgi1_4.h>
#include <DirectXtk/BufferHelpers.h>
#include <DirectXCollision.h>

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
};

struct CB_DirectionLight
{
	Vector3 Direction = { 0.0f, -1.0f, 1.0f };
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
	int pad;									// 4 16
};

struct CB_IBL
{
	int UseIBL = false;							// 4  16byte
	float AmbientOcclusion = 1.0f;				// 4
	Vector2 pad;								// 12	16
};

struct CB_Post
{
	float Gamma = 2.2f;
	Vector3 pad;								// 12	16
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
	ComPtr<IDXGIFactory4> m_pDXGIFactory;		// DXGI팩토리
	ComPtr<IDXGIAdapter3> m_pDXGIAdapter;		// 비디오카드 정보에 접근 가능한 인터페이스
	// 렌더링 파이프라인을 구성하는 필수 객체의 인터페이스
	static ID3D11Device* m_pDevice;						// 디바이스
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;		// 즉시 디바이스 컨텍스트
	ComPtr<IDXGISwapChain> m_pSwapChain;					// 스왑체인
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;	// 렌더링 타겟뷰
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;	// 깊이/스텐실 뷰

	// 렌더링 파이프라인에 적용하는 리소스 객체의 인터페이스
	ComPtr<ID3D11VertexShader> m_pSkeletalMeshVertexShader;	// SkeletalMesh 정점 셰이더.
	ComPtr<ID3D11InputLayout> m_pSkeletalMeshInputLayout;	// SkeletalMesh 입력 레이아웃.	
	ComPtr<ID3D11VertexShader> m_pStaticMeshVertexShader;	// StaticMesh 정점 셰이더.
	ComPtr<ID3D11InputLayout> m_pStaticMeshInputLayout;		// StaticMesh 입력 레이아웃.
	ComPtr<ID3D11PixelShader> m_pPBRPixelShader;		// 픽셀 셰이더.	

	ComPtr<ID3D11VertexShader> m_pEnvironmentVertexShader;	// Environment 정점 셰이더.
	ComPtr<ID3D11PixelShader> m_pEnvironmentPixelShader;	// Environment 정점 셰이더.

	ComPtr<ID3D11SamplerState> m_pSamplerLinear;		// 샘플러 상태.
	ComPtr<ID3D11SamplerState> m_pSamplerClamp;		// 샘플러 상태.

	ComPtr<ID3D11BlendState> m_pAlphaBlendState;		// 블렌드 상태 변경 (반투명처리를위한 블렌드상태)
	ComPtr<ID3D11RasterizerState> m_pRasterizerStateCCW; // 컬 모드 CCW
	ComPtr<ID3D11RasterizerState> m_pRasterizerStateCW; // 컬 모드 CW


	ID3D11Buffer* m_pCBMaterial = nullptr;				// 상수 버퍼: 변환행렬
	ID3D11Buffer* m_pCBTransformW = nullptr;				// 상수 버퍼: 변환행렬
	ID3D11Buffer* m_pCBTransformVP = nullptr;				// 상수 버퍼: 변환행렬
	ID3D11Buffer* m_pCBDirectionLight = nullptr;		// 상수 버퍼: 방향광
	ID3D11Buffer* m_pCBIBL = nullptr; // DirectXTK의 상수버퍼 클래스 활용
	ID3D11Buffer* m_pCBPost = nullptr; // DirectXTK의 상수버퍼 클래스 활용

	ConstantBuffer<CB_MatrixPalette> m_cbMatrixPallete; // DirectXTK의 상수버퍼 클래스 활용


	// 렌더링 파이프라인에 적용하는 정보
	Matrix  m_View;					// 뷰좌표계 공간으로 변환을 위한 행렬.
	Matrix  m_Projection;			// 단위장치좌표계( Normalized Device Coordinate) 공간으로 변환을 위한 행렬.

	Vector3	m_LookAt = { 0.0f, 0.0f, 0.0f };
	Vector3 m_ClearColor = { 0.0f, 0.0f, 0.0f };
	CB_TransformW m_TransformW;
	CB_TransformVP m_TransformVP;
	CB_Marterial m_CpuCbMaterial;
	CB_DirectionLight m_Light;
	CB_IBL m_IBL;
	CB_Post m_Post;

	CB_MatrixPalette m_MatrixPalette;
	D3D11_VIEWPORT m_Viewport;
	HWND m_hWnd = nullptr;
	std::list<StaticMeshInstance*>		m_StaticMeshInstance;		//  렌더링할 모델들의 포인터 저장해둔다. 
	std::list<SkeletalMeshInstance*>	m_SkeletalMeshInstance;		//  렌더링할 모델들의 포인터 저장해둔다. 
	std::list<SkeletalMeshComponent*>	m_SkeletalMeshComponents;		//  렌더링할 모델들의 포인터 저장해둔다.
	std::list<StaticMeshComponent*>		m_StaticMeshComponents;		//  렌더링할 모델들의 포인터 저장해둔다

	std::list<IImGuiRenderable*>		m_ImGuiRenders;
	std::list<DebugRay>					m_DebugDrawLines;
	std::weak_ptr<CameraComponent>		m_pCamera;
	std::weak_ptr<EnvironmentMeshComponent> m_pEnvironmentMeshComponent;

	BoundingFrustum m_Frustum;
	int m_nDrawComponentCount = 0;
	bool m_bFreezeCulling = false;
    bool m_bWorkCulling = true;
	bool m_bDrawDebugCulling = false;
	bool m_bDrawDebugCollision = false;
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

	//컴포넌트 등록하기
	void AddSkeletalMeshComponent(SkeletalMeshComponent* pSkeletalMeshComponent);
	void RemoveSkeletalMeshComponent(SkeletalMeshComponent* pSkeletalMeshComponent);
	void AddStaticMeshComponent(StaticMeshComponent* pStaticMeshComponent);
	void RemoveStaticMeshComponent(StaticMeshComponent* pStaticMeshComponent);

	// ImGui 디버깅 정보 텍스트 그리기
	void AddImguiRenderable(IImGuiRenderable* pIImGuiRenderable);
	void RemoveImguiRenderable(IImGuiRenderable* pIImGuiRenderable);
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
private:
	void CreatePBR();
	void CreateIBL();
	void CreateConstantBuffer();
	void CreateSamplerState();
	void CreateRasterizerState();
	void CreateBlendState();
	void CreateEnvironment();

	void AddMeshInstance(SkeletalMeshComponent* pModel);
	void AddMeshInstance(StaticMeshComponent* pModel);

	void RenderDebugDraw();
	void RenderImGui();
	void RenderSkeletalMeshInstance();
	void RenderStaticMeshInstance();
	void RenderEnvironment();
};
