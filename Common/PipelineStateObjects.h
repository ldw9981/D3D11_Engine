#pragma once
/*
	상황에 따라 각기 다른 RenderState를 가지는 PipelineStateObject를 관리하는 클래스입니다.
	예) DebugDraw, Default, ShadowMap, DepthOnly, ...
*/
class PipelineStateObjects
{
public:
	PipelineStateObjects() {};
	~PipelineStateObjects() {};
	
	ComPtr<ID3D11DepthStencilState> m_pDepthStencilState = nullptr;	// 깊이/스텐실 상태
	ComPtr<ID3D11RasterizerState>	m_pRasterizerState = nullptr;	// 래스터라이저 상태	
	ComPtr<ID3D11SamplerState>		m_pSamplerLinear = nullptr;		// 샘플러 상태.
	ComPtr<ID3D11BlendState>		m_pAlphaBlendState = nullptr;		// 블렌드 상태 
};

