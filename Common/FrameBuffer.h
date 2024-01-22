#pragma once

// ���� ����۴������ �������� �ƴ�  ������ �ؽ�ó�� �������� �ϰ� �ؽ�ó�� ���̴����� ���μҽ��� ����ϱ����� D3D11Ŭ���� ���� 
class FrameBuffer
{
public:
	FrameBuffer() {};
	~FrameBuffer() {};

	ComPtr<ID3D11Texture2D> RenderTargetTexture;	// ����Ÿ�ٿ� �ؽ�ó, ����/���ٽǿ� �ؽ�ó�� ũ�� �����ؾ���	
	ComPtr<ID3D11RenderTargetView> RTV;				// ����Ÿ������ �������� Ŭ����
	ComPtr<ID3D11ShaderResourceView> SRV;			// ����Ÿ�ٿ� �ؽ�ó�� ���̴����� ����ϱ����� Ŭ����

	ComPtr<ID3D11Texture2D> DepthStencilTexture;	// ����/���ٽǿ� �ؽ�ó, ����Ÿ�ٿ� �ؽ�ó ũ��� �����ؾ���
	ComPtr<ID3D11DepthStencilView> DSV;				// �ؽ�ó�� ����/���ٽǷ� �������� Ŭ����
	UINT Width=0;
	UINT Height=0;								// ũ������
	UINT Samples=0;
};

