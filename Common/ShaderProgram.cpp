#include "pch.h"
#include "ShaderProgram.h"

void ShaderProgram::SetShader(ID3D11DeviceContext* context)
{
	context->IASetInputLayout(IL.Get());
	context->VSSetShader(VS.Get(), nullptr, 0);
	context->PSSetShader(PS.Get(), nullptr, 0);
}
