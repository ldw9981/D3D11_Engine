#include "pch.h"
#include "Material.h"
#include "D3DRenderManager.h"
#include "Helper.h"
#include <assimp/material.h>
#include "ResourceManager.h"


struct MaterialProperty
{
	std::string Name;
	int pI;
	float pF;
	Vector2 pF2;
	Vector3 pF3;
	Vector4 pF4;
	std::string pS;
};

Material::Material()
{

}

Material::~Material()
{
	
}

void Material::TestMaterialPropery(const aiMaterial* pMaterial)
{
	std::list<MaterialProperty> properties;
	for (size_t i = 0; i < pMaterial->mNumProperties; i++)
	{
		aiMaterialProperty* pProperty = pMaterial->mProperties[i];

		MaterialProperty& item = properties.emplace_back();

		item.Name = pProperty->mKey.C_Str();

		if (pProperty->mType == aiPTI_Float) {
			if (pProperty->mDataLength == 4)
				item.pF = *(float*)pProperty->mData;
			else if (pProperty->mDataLength == 8)
				item.pF2 = *(Vector2*)pProperty->mData;
			else if (pProperty->mDataLength == 12)
				item.pF3 = *(Vector3*)pProperty->mData;
			else if (pProperty->mDataLength == 16)
				item.pF4 = *(Vector4*)pProperty->mData;
		}
		else if (pProperty->mType == aiPTI_String) {
			item.pS = pProperty->mData;
		}
		else if (pProperty->mType == aiPTI_Integer) {
			item.pI = *(int*)pProperty->mData;
		}
	}
}

void Material::TestTextureProperty(const aiMaterial* pMaterial, std::vector<std::pair<aiTextureType, std::string>>& textureProperties)
{
	// Diffuse
	
	std::filesystem::path path;

	int max_size = aiTextureType_TRANSMISSION + 1;
	textureProperties.resize(max_size);

	for (size_t i = 0; i < max_size ; i++)
	{
		textureProperties[i].first = (aiTextureType)i;
		aiString texturePath;
		if (AI_SUCCESS == pMaterial->GetTexture((aiTextureType)i, 0, &texturePath))
		{
			path = ToWString(string(texturePath.C_Str()));	
			textureProperties[i].second = path.filename().string();
		}
	}	
}

void Material::Create(const aiMaterial* pMaterial)
{
	// Diffuse
	aiString texturePath;
	wstring basePath=L"../Resource/";
	std::filesystem::path path;
	wstring finalPath;
	string name = pMaterial->GetName().C_Str();
	
	//TestMaterialPropery(pMaterial);

	std::vector<std::pair<aiTextureType, std::string>> textureProperties;	
	TestTextureProperty(pMaterial, textureProperties);
	
	aiColor3D color(1.f, 1.f, 1.f);
	if(AI_SUCCESS == pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color))
	{
		m_Color ={ color.r, color.g, color.b , 1};
	}	

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath)) 
	{
		path = ToWString(string(texturePath.C_Str()));		
		finalPath = basePath + path.filename().wstring();
		
		m_pBaseColor = ResourceManager::Instance->CreateMaterialTexture(finalPath);		
	}

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_NORMALS, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		m_pNormal = ResourceManager::Instance->CreateMaterialTexture(finalPath);	
	}

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();		
		m_pSpecular = ResourceManager::Instance->CreateMaterialTexture(finalPath);			
	}

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		m_pEmissive = ResourceManager::Instance->CreateMaterialTexture(finalPath);
	}

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_OPACITY, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		m_pOpacity = ResourceManager::Instance->CreateMaterialTexture(finalPath);		
	}
}

MaterialTexture::MaterialTexture()
{
}

MaterialTexture::~MaterialTexture()
{
}

void MaterialTexture::Create(const std::wstring& filePath)
{
	HR_T(CreateTextureFromFile(D3DRenderManager::m_pDevice, filePath.c_str(), &m_pTextureSRV));
	m_FilePath = filePath;
}
