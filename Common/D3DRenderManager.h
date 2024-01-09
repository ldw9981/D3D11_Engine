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
	ComPtr<IDXGIFactory4> m_pDXGIFactory;		// DXGI���丮
	ComPtr<IDXGIAdapter3> m_pDXGIAdapter;		// ����ī�� ������ ���� ������ �������̽�
	// ������ ������������ �����ϴ� �ʼ� ��ü�� �������̽�
	static ID3D11Device* m_pDevice;						// ����̽�
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;		// ��� ����̽� ���ؽ�Ʈ
	ComPtr<IDXGISwapChain> m_pSwapChain;					// ����ü��
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;	// ������ Ÿ�ٺ�
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;	// ����/���ٽ� ��

	// ������ ���������ο� �����ϴ� ���ҽ� ��ü�� �������̽�
	ComPtr<ID3D11VertexShader> m_pSkeletalMeshVertexShader;	// SkeletalMesh ���� ���̴�.
	ComPtr<ID3D11InputLayout> m_pSkeletalMeshInputLayout;	// SkeletalMesh �Է� ���̾ƿ�.	
	ComPtr<ID3D11VertexShader> m_pStaticMeshVertexShader;	// StaticMesh ���� ���̴�.
	ComPtr<ID3D11InputLayout> m_pStaticMeshInputLayout;		// StaticMesh �Է� ���̾ƿ�.
	ComPtr<ID3D11PixelShader> m_pPBRPixelShader;		// �ȼ� ���̴�.	

	ComPtr<ID3D11VertexShader> m_pEnvironmentVertexShader;	// Environment ���� ���̴�.
	ComPtr<ID3D11PixelShader> m_pEnvironmentPixelShader;	// Environment ���� ���̴�.

	ComPtr<ID3D11SamplerState> m_pSamplerLinear;		// ���÷� ����.
	ComPtr<ID3D11SamplerState> m_pSamplerClamp;		// ���÷� ����.

	ComPtr<ID3D11BlendState> m_pAlphaBlendState;		// ���� ���� ���� (������ó�������� �������)
	ComPtr<ID3D11RasterizerState> m_pRasterizerStateCCW; // �� ��� CCW
	ComPtr<ID3D11RasterizerState> m_pRasterizerStateCW; // �� ��� CW


	ID3D11Buffer* m_pCBMaterial = nullptr;				// ��� ����: ��ȯ���
	ID3D11Buffer* m_pCBTransformW = nullptr;				// ��� ����: ��ȯ���
	ID3D11Buffer* m_pCBTransformVP = nullptr;				// ��� ����: ��ȯ���
	ID3D11Buffer* m_pCBDirectionLight = nullptr;		// ��� ����: ���Ɽ
	ID3D11Buffer* m_pCBIBL = nullptr; // DirectXTK�� ������� Ŭ���� Ȱ��
	ID3D11Buffer* m_pCBPost = nullptr; // DirectXTK�� ������� Ŭ���� Ȱ��

	ConstantBuffer<CB_MatrixPalette> m_cbMatrixPallete; // DirectXTK�� ������� Ŭ���� Ȱ��


	// ������ ���������ο� �����ϴ� ����
	Matrix  m_View;					// ����ǥ�� �������� ��ȯ�� ���� ���.
	Matrix  m_Projection;			// ������ġ��ǥ��( Normalized Device Coordinate) �������� ��ȯ�� ���� ���.

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
	std::list<StaticMeshInstance*>		m_StaticMeshInstance;		//  �������� �𵨵��� ������ �����صд�. 
	std::list<SkeletalMeshInstance*>	m_SkeletalMeshInstance;		//  �������� �𵨵��� ������ �����صд�. 
	std::list<SkeletalMeshComponent*>	m_SkeletalMeshComponents;		//  �������� �𵨵��� ������ �����صд�.
	std::list<StaticMeshComponent*>		m_StaticMeshComponents;		//  �������� �𵨵��� ������ �����صд�

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
