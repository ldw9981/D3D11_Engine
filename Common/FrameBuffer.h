#pragma once

// 최종 백버퍼대상으로 렌더링이 아닌  생성한 텍스처에 렌더링을 하고 텍스처를 셰이더에서 매핑소스로 사용하기위한 D3D11클래스 모음 
class FrameBuffer
{
public:
	FrameBuffer() {};
	~FrameBuffer() {};

	ComPtr<ID3D11Texture2D> RenderTargetTexture;	// 렌더타겟용 텍스처, 깊이/스텐실용 텍스처와 크기 동일해야함	
	ComPtr<ID3D11RenderTargetView> RTV;				// 렌더타겟으로 쓰기위한 클래스
	ComPtr<ID3D11ShaderResourceView> SRV;			// 렌더타겟용 텍스처를 셰이더에서 사용하기위한 클래스

	ComPtr<ID3D11Texture2D> DepthStencilTexture;	// 깊이/스텐실용 텍스처, 렌더타겟용 텍스처 크기와 동일해야함
	ComPtr<ID3D11DepthStencilView> DSV;				// 텍스처를 깊이/스텐실로 쓰기위한 클래스
	UINT Width, Height;								// 크기정보
	UINT Samples;
};

