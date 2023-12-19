#pragma once
#include <dxgi.h>

#include <DirectXtk/BufferHelpers.h>

using namespace DirectX::SimpleMath;
using namespace DirectX;

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
	Vector3 Direction = { 0.5f, 0.0f, 0.8f };
	float pad0 = 0.0f;
	Vector4 Ambient = { 0.1f,0.1f,0.1f,0.1f };
	Vector4 Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 Specular = { 1.0f,1.0f,1.0f,1.0f };
	Vector3 EyePosition = {};
	float pad1 = 0.0f;
};

struct CB_MatrixPalette
{
	Math::Matrix Array[128];
};

struct CB_Marterial
{
	Math::Vector4 Ambient = { 1.0f,1.0f,1.0f,1.0f };	// 16byte
	Math::Vector4 Diffuse = { 1.0f,1.0f,1.0f,1.0f };	// 16byte
	Math::Vector4 Specular = { 1.0f,1.0f,1.0f,1.0f };  // 16byte
	Math::Vector4 Emissive = { 1.0f,1.0f,1.0f,1.0f };  // 16byte
	float  SpecularPower = 80;					// 4
	int UseDiffuseMap = true;					// 4
	int UseNormalMap = true;					// 4
	int UseSpecularMap = true;                  // 4 16byte			
	int UseEmissiveMap = true;					// 4
	int UseOpacityMap = true;					// 4
	Math::Vector2 pad6;							// 8  16byte
};

class StaticMeshModel;
class SkeletalMeshModel;
class Material;

class IRenderable
{
public:
	virtual void Render(ID3D11DeviceContext* pDeviceContext) = 0;
};

class IImGuiRenderable
{
public:
	virtual void ImGuiRender() = 0;
};


class D3DRenderManager
{
public:
	D3DRenderManager();
	~D3DRenderManager();

	static D3DRenderManager* Instance;
	// 렌더링 파이프라인을 구성하는 필수 객체의 인터페이스
	static ID3D11Device* m_pDevice;						// 디바이스
	ID3D11DeviceContext* m_pDeviceContext = nullptr;		// 즉시 디바이스 컨텍스트
	IDXGISwapChain* m_pSwapChain = nullptr;					// 스왑체인
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;	// 렌더링 타겟뷰
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;	// 깊이/스텐실 뷰


	// 렌더링 파이프라인에 적용하는 리소스 객체의 인터페이스
	ID3D11VertexShader* m_pSkeletalMeshVertexShader = nullptr;	// SkeletalMesh 정점 셰이더.
	ID3D11InputLayout* m_pSkeletalMeshInputLayout = nullptr;	// SkeletalMesh 입력 레이아웃.
	
	ID3D11VertexShader* m_pStaticMeshVertexShader = nullptr;	// StaticMesh 정점 셰이더.
	ID3D11InputLayout* m_pStaticMeshInputLayout = nullptr;		// StaticMesh 입력 레이아웃.

	ID3D11PixelShader* m_pPixelShader = nullptr;		// 픽셀 셰이더.	
	ID3D11SamplerState* m_pSamplerLinear = nullptr;		// 샘플러 상태.
	ID3D11BlendState* m_pAlphaBlendState = nullptr;		// 블렌드 상태 변경 (반투명처리를위한 블렌드상태)
	ID3D11Buffer* m_pGpuCbMaterial = nullptr;				// 상수 버퍼: 변환행렬
	ID3D11Buffer* m_pCBTransformW = nullptr;				// 상수 버퍼: 변환행렬
	ID3D11Buffer* m_pCBTransformVP = nullptr;				// 상수 버퍼: 변환행렬
	ID3D11Buffer* m_pCBDirectionLight = nullptr;		// 상수 버퍼: 방향광
	ConstantBuffer<CB_MatrixPalette> m_cbMatrixPallete; // DirectXTK의 상수버퍼 클래스 활용

	// 렌더링 파이프라인에 적용하는 정보
	UINT m_VertexBufferStride = 0;						// 버텍스 하나의 크기.
	UINT m_VertexBufferOffset = 0;						// 버텍스 버퍼의 오프셋.
	int m_nIndices = 0;				// 인덱스 개수.
	Matrix	m_World;				// 월드좌표계 공간으로 변환을 위한 행렬.
	Matrix  m_View;					// 뷰좌표계 공간으로 변환을 위한 행렬.
	Matrix  m_Projection;			// 단위장치좌표계( Normalized Device Coordinate) 공간으로 변환을 위한 행렬.

	Vector3 m_ClearColor = { 0.0f, 0.0f, 0.0f };
	Vector3 m_Rotation = Vector3(0.0f, 0.0f,0.0f);
	Vector3 m_CameraPos = Vector3(0.0f, 100.0f, -1000.0f);

	CB_TransformW m_TransformW;
	CB_TransformVP m_TransformVP;
	CB_Marterial m_CpuCbMaterial;
	CB_DirectionLight m_Light;
	CB_MatrixPalette m_MatrixPalette;
	D3D11_VIEWPORT m_Viewport;

	HWND m_hWnd = nullptr;
	float m_MeshScale = 1.0f;
	
	void SetImGuiRender(IImGuiRenderable* val) { m_pImGuiRender = val; }
	std::list<SkeletalMeshModel*> m_Models;		//  렌더링할 모델들의 포인터 저장해둔다. 
	std::list<StaticMeshModel*> m_StaticModels;		//  렌더링할 모델들의 포인터 저장해둔다. 
	// 렌더링 이후에 목록은 사라진다.
	void AddModel(SkeletalMeshModel* pModel);
	void AddModel(StaticMeshModel* pModel);
public:
	bool Initialize(HWND Handle,UINT Width, UINT Height);
	void Uninitialize();

	void Update();
	void Render();

	void CreateSkeletalMesh_VS_IL();
	void CreateStaticMesh_VS_IL();
	void CreatePS();
	void ApplyMaterial(Material* pMaterial);
private:
	IImGuiRenderable* m_pImGuiRender = nullptr;
};
