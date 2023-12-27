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
	int UseIBL = false;							// 4  16byte
};

class StaticMeshComponent;
class SkeletalMeshComponent;
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

class StaticMeshInstance;
class SkeletalMeshInstance;
class SkeletalMeshComponent;
class CameraComponent;
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
	ID3D11DeviceContext* m_pDeviceContext = nullptr;		// ��� ����̽� ���ؽ�Ʈ
	IDXGISwapChain* m_pSwapChain = nullptr;					// ����ü��
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;	// ������ Ÿ�ٺ�
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;	// ����/���ٽ� ��


	// ������ ���������ο� �����ϴ� ���ҽ� ��ü�� �������̽�
	ID3D11VertexShader* m_pSkeletalMeshVertexShader = nullptr;	// SkeletalMesh ���� ���̴�.
	ID3D11InputLayout* m_pSkeletalMeshInputLayout = nullptr;	// SkeletalMesh �Է� ���̾ƿ�.
	
	ID3D11VertexShader* m_pStaticMeshVertexShader = nullptr;	// StaticMesh ���� ���̴�.
	ID3D11InputLayout* m_pStaticMeshInputLayout = nullptr;		// StaticMesh �Է� ���̾ƿ�.

	ID3D11PixelShader* m_pPixelShader = nullptr;		// �ȼ� ���̴�.	
	ID3D11SamplerState* m_pSamplerLinear = nullptr;		// ���÷� ����.
	ID3D11BlendState* m_pAlphaBlendState = nullptr;		// ���� ���� ���� (������ó�������� �������)
	ID3D11Buffer* m_pGpuCbMaterial = nullptr;				// ��� ����: ��ȯ���
	ID3D11Buffer* m_pCBTransformW = nullptr;				// ��� ����: ��ȯ���
	ID3D11Buffer* m_pCBTransformVP = nullptr;				// ��� ����: ��ȯ���
	ID3D11Buffer* m_pCBDirectionLight = nullptr;		// ��� ����: ���Ɽ
	ConstantBuffer<CB_MatrixPalette> m_cbMatrixPallete; // DirectXTK�� ������� Ŭ���� Ȱ��

	// ������ ���������ο� �����ϴ� ����
	UINT m_VertexBufferStride = 0;						// ���ؽ� �ϳ��� ũ��.
	UINT m_VertexBufferOffset = 0;						// ���ؽ� ������ ������.
	int m_nIndices = 0;				// �ε��� ����.
	Matrix	m_World;				// ������ǥ�� �������� ��ȯ�� ���� ���.
	Matrix  m_View;					// ����ǥ�� �������� ��ȯ�� ���� ���.
	Matrix  m_Projection;			// ������ġ��ǥ��( Normalized Device Coordinate) �������� ��ȯ�� ���� ���.

	Vector3	m_LookAt = { 0.0f, 0.0f, 0.0f };
	Vector3 m_ClearColor = { 0.0f, 0.0f, 0.0f };

	CB_TransformW m_TransformW;
	CB_TransformVP m_TransformVP;
	CB_Marterial m_CpuCbMaterial;
	CB_DirectionLight m_Light;
	CB_MatrixPalette m_MatrixPalette;
	D3D11_VIEWPORT m_Viewport;

	HWND m_hWnd = nullptr;

	std::list<StaticMeshInstance*> m_StaticMeshInstance;		//  �������� �𵨵��� ������ �����صд�. 
	std::list<SkeletalMeshInstance*> m_SkeletalMeshInstance;		//  �������� �𵨵��� ������ �����صд�. 


	std::list<SkeletalMeshComponent*> m_SkeletalMeshComponents;		//  �������� �𵨵��� ������ �����صд�.
	std::list<StaticMeshComponent*> m_StaticMeshComponents;		//  �������� �𵨵��� ������ �����صд�.

	IImGuiRenderable* m_pImGuiRender = nullptr;
	std::weak_ptr<CameraComponent> m_pCamera;
	BoundingFrustum m_Frustum;
	int m_nDrawComponentCount = 0;
	bool m_bFreezeCulling = false;
    bool m_bWorkCulling = true;
	bool m_bDrawDebug = true;
public:

	bool Initialize(HWND Handle,UINT Width, UINT Height);
	void Uninitialize();

	void Update();
	void Render();
	void ApplyMaterial(Material* pMaterial);

	void GetVideoMemoryInfo(std::string& out);
	void GetSystemMemoryInfo(std::string& out);

	void AddDebugStringToImGuiWindow(const std::string& header, const std::string& str);
	void AddDebugMatrixToImGuiWindow(const std::string& header,const Matrix& mat);
	void AddDebugVector4ToImGuiWindow(const std::string& header, const Vector4& value);
	void AddDebugVector3ToImGuiWindow(const std::string& header, const Vector3& value);
	void AddDebugVector2ToImGuiWindow(const std::string& header, const Vector2& value);
	void AddDebugFloatToImGuiWindow(const std::string& header, const float& value);

	std::weak_ptr<CameraComponent> Camera() const { return m_pCamera; }
	void SetCamera(std::weak_ptr<CameraComponent> val) { m_pCamera = val; }
	void SetImGuiRender(IImGuiRenderable* val) { m_pImGuiRender = val; }
	bool GetFreezeCulling() const { return m_bFreezeCulling; }
	void SetFreezeCulling(bool val) { m_bFreezeCulling = val; }
private:
	void CreateSkeletalMesh_VS_IL();
	void CreateStaticMesh_VS_IL();
	void CreatePS();
	void CreateIBL();

	void AddMeshInstance(SkeletalMeshComponent* pModel);
	void AddMeshInstance(StaticMeshComponent* pModel);

	void RenderDebugDraw();
	void RenderImGui();
	void RenderSkeletalMeshInstance();
	void RenderStaticMeshInstance();


	ComPtr<ID3D11SamplerState> CreateSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode) const;
};
