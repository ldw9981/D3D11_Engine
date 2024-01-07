#pragma once


class TextureResource
{
public:
	TextureResource();
	~TextureResource();

	std::wstring m_FilePath;
	ComPtr<ID3D11ShaderResourceView> m_pTextureSRV = nullptr;	// �ؽ�ó ���ҽ� ��.
	void Create(const std::wstring& filePath);
};
