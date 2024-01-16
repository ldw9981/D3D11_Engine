#pragma once
class ShaderProgram
{
public:
	ShaderProgram() {};
	~ShaderProgram() {};

	ComPtr<ID3D11VertexShader> VS;	
	ComPtr<ID3D11PixelShader> PS;	
	ComPtr<ID3D11InputLayout> IL;		

	void SetShader(ID3D11DeviceContext* context);
};

