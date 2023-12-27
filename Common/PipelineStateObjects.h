#pragma once
/*
	��Ȳ�� ���� ���� �ٸ� RenderState�� ������ PipelineStateObject�� �����ϴ� Ŭ�����Դϴ�.
	��) DebugDraw, Default, ShadowMap, DepthOnly, ...
*/
class PipelineStateObjects
{
public:
	PipelineStateObjects() {};
	~PipelineStateObjects() {};
	
	ComPtr<ID3D11DepthStencilState> m_pDepthStencilState = nullptr;	// ����/���ٽ� ����
	ComPtr<ID3D11RasterizerState>	m_pRasterizerState = nullptr;	// �����Ͷ����� ����	
	ComPtr<ID3D11SamplerState>		m_pSamplerLinear = nullptr;		// ���÷� ����.
	ComPtr<ID3D11BlendState>		m_pAlphaBlendState = nullptr;		// ���� ���� 
};

