#pragma once


class TextureResource
{
public:
	TextureResource();
	~TextureResource();

	std::wstring m_FilePath;
	ComPtr<ID3D11ShaderResourceView> m_pTextureSRV = nullptr;	// 텍스처 리소스 뷰.
	void Create(const std::wstring& filePath);
};
