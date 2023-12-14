#pragma once
#include "D3DRenderer.h"


struct aiMaterial;

class Material
{
public:
	Material();
	~Material();

	void Create(ID3D11Device* device,aiMaterial* pMaterial);
	
	std::wstring m_FilePathBaseColor;
	std::wstring m_FilePathNormal;
	std::wstring m_FilePathSpecular;
	std::wstring m_FilePathEmissive;
	std::wstring m_FilePathOpacity;
	std::wstring m_FilePathRoughness;
	std::wstring m_FilePathMetalness;


	Math::Vector4 m_Color;	
	
	ID3D11ShaderResourceView* m_pBaseColorRV = nullptr;	// �ؽ�ó ���ҽ� ��.
	ID3D11ShaderResourceView* m_pNormalRV = nullptr;	// �븻�� ���ҽ� ��.
	ID3D11ShaderResourceView* m_pSpecularRV = nullptr;	// ����ŧ���� ���ҽ� ��.
	ID3D11ShaderResourceView* m_pEmissiveRV = nullptr;	// �̹̽ú�� ���ҽ� ��.	
	ID3D11ShaderResourceView* m_pOpacityRV = nullptr;	// ����� ���ҽ� ��.	

	ID3D11ShaderResourceView* m_pRoughnessRV = nullptr;	// ��ĥ��
	ID3D11ShaderResourceView* m_pMetalnessRV = nullptr;	// �ݼӼ�	

	//void ApplyDeviceContext(ID3D11DeviceContext* deviceContext, CB_Marterial* cpuCbMaterial, ID3D11Buffer* gpuCbMarterial, ID3D11BlendState* alphaBlendState);
};

