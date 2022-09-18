#include <d3d11.h>
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Renderer/Renderer.hpp"

Shader::Shader(const ShaderConfig& config)
	:m_config(config)
{
}

Shader::~Shader()
{
	DX_SAFE_RELEASE(m_vertexShader);
	DX_SAFE_RELEASE(m_pixelShader);
	DX_SAFE_RELEASE(m_inputLayout);
}

const std::string& Shader::GetName() const
{
	return m_config.m_name;
}
