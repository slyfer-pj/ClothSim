#include <d3d11.h>
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/Renderer.hpp"

Texture::~Texture()
{
	DX_SAFE_RELEASE(m_texture);
	DX_SAFE_RELEASE(m_shaderResourceView);
}
