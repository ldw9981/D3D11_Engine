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
	std::list<SkeletalMeshModel*> m_Models;		//  �������� �𵨵��� ������ �����صд�. 
	std::list<StaticMeshModel*> m_StaticModels;		//  �������� �𵨵��� ������ �����صд�. 
	// ������ ���Ŀ� ����� �������.
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
