#pragma once

struct aiMaterial;


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


class Material
{
public:
	Material();
	~Material();

	void Create(ID3D11Device* device,aiMaterial* pMaterial);
	
	std::wstring m_FilePathDiffuse;
	std::wstring m_FilePathNormal;
	std::wstring m_FilePathSpecular;
	std::wstring m_FilePathEmissive;
	std::wstring m_FilePathOpacity;

	Math::Vector4 m_Color;	
	
	ID3D11ShaderResourceView* m_pDiffuseRV = nullptr;	// �ؽ�ó ���ҽ� ��.
	ID3D11ShaderResourceView* m_pNormalRV = nullptr;	// �븻�� ���ҽ� ��.
	ID3D11ShaderResourceView* m_pSpecularRV = nullptr;	// ����ŧ���� ���ҽ� ��.
	ID3D11ShaderResourceView* m_pEmissiveRV = nullptr;	// �̹̽ú�� ���ҽ� ��.	
	ID3D11ShaderResourceView* m_pOpacityRV = nullptr;	// ����� ���ҽ� ��.	

	void ApplyDeviceContext(ID3D11DeviceContext* deviceContext, CB_Marterial* cpuCbMaterial, ID3D11Buffer* gpuCbMarterial, ID3D11BlendState* alphaBlendState);
};

