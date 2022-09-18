#pragma once
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/IntVec2.hpp"
#include <vector>

#define DX_SAFE_RELEASE(dxObject)	\
{									\
	if ((dxObject) != nullptr)		\
	{								\
		(dxObject)->Release();		\
		(dxObject) = nullptr;		\
	}								\
}

#undef OPAQUE

class Window;
class Texture;
class BitmapFont;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11RasterizerState;
struct ID3D11InputLayout;
struct ID3D11Buffer;
struct ID3D11BlendState;
struct ID3D11SamplerState;
struct ID3D11DeviceChild;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11Resource;
struct ID3D11UnorderedAccessView;
struct ID3D11ComputeShader;
class VertexBuffer;
class Shader;
class ConstantBuffer;
class Image;
class IndexBuffer;
struct Vertex_PNCU;

enum class BlendMode
{
	ALPHA,
	ADDITIVE,
	OPAQUE
};

enum class CullMode
{
	NONE,
	FRONT,
	BACK,
};

enum class FillMode
{
	SOLID,
	WIREFRAME,
};

enum class WindingOrder
{
	CLOCKWISE,
	COUNTERCLOCKWISE,
};

enum class DepthTest
{
	ALWAYS,
	NEVER,
	EQUAL,
	NOTEQUAL,
	LESS,
	LESSEQUAL,
	GREATER,
	GREATEREQUAL,
};

enum class SamplerMode
{
	POINTCLAMP,
	POINTWRAP,
	BILINEARCLAMP,
	BILINEARWRAP,
};

struct SpotLight
{
	Vec3 lightPosition;
	float lightConeHalfAngle;
	Vec3 lightDirection;
	float padding;
	float lightColor[4];
};

struct RendererConfig
{
	Window* m_window;
};

class Renderer
{
public:
	Renderer(const RendererConfig& config);
	void Startup();
	void BeginFrame();
	void EndFrame();
	void Shutdown();

	void ClearScreen(const Rgba8& clearColor);
	void ClearDepth(float value = 1.0f);
	void BeginCamera(const Camera& camera);
	void EndCamera(const Camera& camera);

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

	void DrawVertexArray(int numVertexes, const Vertex_PCU* vertexes);
	void DrawVertexArray(int numVertexes, const Vertex_PNCU* vertexes);
	void DrawIndexed(int numIndices);
	//void CreateRenderContext();

	Texture* CreateOrGetTextureFromFile(char const* imageFilePath);
	BitmapFont* CreateOrGetBitmapFont(const char* bitmapFontFilePathWithNoExtension);
	Shader* CreateOrGetShader(const char* shaderName);
	const Shader* GetDefaultShader() const;
	void BindTexture(const Texture* texture);
	VertexBuffer* CreateVertexBuffer(const size_t size, unsigned int stride);
	void CopyCPUToGPU(const void* data, size_t size, VertexBuffer* vbo);
	void BindVertexBuffer(VertexBuffer* vbo);
	IndexBuffer* CreateIndexBuffer(const size_t size);
	void CopyCPUToGPU(const void* data, size_t size, IndexBuffer* ibo);
	void BindIndexBuffer(IndexBuffer* ibo);
	ConstantBuffer* CreateConstantBuffer(const size_t size);
	void CopyCPUToGPU(const void* data, size_t size, ConstantBuffer* cbo);
	void BindConstantBuffer(int slot, ConstantBuffer* cbo);
	void BindConstantBufferToComputeShader(int slot, ConstantBuffer* cbo);
	bool CompileShaderToByteCode(std::vector<unsigned char>& outByteCode, char const* name, char const* source, char const* entryPoint, char const* target);
	void CreateAndCompileComputeShader(char const* shaderName, ID3D11ComputeShader** pComputeShader);
	void CreateD3DShaderResource(ID3D11Buffer** pBuffer, const size_t totalBufferSize, const size_t stride, const void* initialData);
	void CopyCPUToGPU(const void* data, const size_t size, ID3D11Buffer* pDestinationBuffer);
	void CreateShaderResourceView(ID3D11ShaderResourceView** pResourceView, ID3D11Buffer* pShaderResource);
	void CreateD3DUnorderedAccessBuffer(ID3D11Buffer** pBuffer, const size_t totalBufferSize, const size_t stride, const void* initialData);
	void CreateUnorderedAccessView(ID3D11UnorderedAccessView** pResourceView, ID3D11Buffer* pUnorderedResource);
	void BindComputeShaderAndComputeResources(ID3D11ComputeShader* pComputeShader, ID3D11ShaderResourceView* pInputSRV, ID3D11UnorderedAccessView* pOutputUAV);
	void DispatchComputeShader(int numThreadGroupX, int numThreadGroupY, int numThreadGroupZ);
	void CopyOverResourcesInGPU(ID3D11Buffer* pDestResource, ID3D11Buffer* pSourceResource);
	void CopyGPUToCPU(void* data, const size_t copySize, ID3D11Buffer* pSourceBuffer);

	void BindShaderByName(const char* shaderName);
	void BindShader(Shader* shader);
	void SetBlendMode(BlendMode blendMode);
	void SetModelMatrix(const Mat44& modelMatrix);
	void SetModelColor(const Rgba8& modelColor);
	void SetRasterizerState(CullMode cullMode, FillMode fillMode, WindingOrder windingOrder);
	void SetDepthStencilState(DepthTest depthTest, bool writeDepth);
	void SetSamplerMode(SamplerMode samplerMode);

	void SetSunDirection(const Vec3& direction);
	void SetSunIntensity(float intensity);
	void SetAmbientIntensity(float intensity);
	void SetSpotlightProperties(const SpotLight& info);

private:
	Texture* GetTextureForFileName(char const* imageFilePath);
	Texture* CreateTextureFromFile(char const* imageFilePath, bool saveTexture = true);
	Texture* CreateTextureFromData(char const* name, IntVec2 dimensions, int bytesPerTexel, uint8_t* texelData);
	Texture* CreateTextureFromImage(const Image& image);

	BitmapFont* GetBitmpaFontForFileName(const char* fontFilePath);
	BitmapFont* CreateBitmapFont(const char* fontFilePath);

	void CreateD3DDeviceAndSwapChain();
	void CreateRenderTargetView();
	void DefineViewport(const AABB2& camViewport);
	void CreateAndSetRasterizerState();
	void CreateInputLayout(Shader* shader, bool isLitShader);

	Shader* GetShaderForName(const char* shaderName);
	Shader* CreateShader(const char* shaderName);
	Shader* CreateShader(char const* shaderName, char const* shaderSource);

	void CreateD3DVertexBuffer(const size_t size, VertexBuffer* vbo);
	void CreateD3DConstantBuffer(const size_t size, ConstantBuffer* cbo);
	void CreateD3DIndexBuffer(const size_t size, IndexBuffer* ibo);

	void DrawVertexBuffer(VertexBuffer* vbo, int vertexCount, int vertexOffset = 0);
	//void CreateAndBindSamplerState();
	void SetDebugName(ID3D11DeviceChild* object, const char* name);

	void CreateDepthStencilTextureAndView();

private:
	RendererConfig m_config;
	std::vector<Texture*> m_loadedTextures;
	Texture* m_defaultTexture = nullptr;
	std::vector<BitmapFont*> m_loadedFonts;
	std::vector<Shader*> m_loadedShaders;
	const Shader* m_currentShader = nullptr;
	const Shader* m_defaultShader = nullptr;
	std::vector<uint8_t> m_vertexShaderByteCode = {};
	std::vector<uint8_t> m_pixelShaderByteCode = {};
	VertexBuffer* m_immediateVBO_PCU = nullptr;
	VertexBuffer* m_immediateVBO_PNCU = nullptr;
	ConstantBuffer* m_lightCBO = nullptr;
	ConstantBuffer* m_cameraCBO = nullptr;
	ConstantBuffer* m_modelCBO = nullptr;
	Mat44 m_modelMatrix;
	float m_modelColor[4] = { 0.f };
	Vec3 m_sunDirection = Vec3(0.f, 0.f, -1.f);
	float m_sunIntensity = 0.f;
	float m_ambientIntensity = 1.f;
	SpotLight m_spotlight;

private:
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
	IDXGISwapChain* m_swapChain = nullptr;
	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	ID3D11RasterizerState* m_rasterizerState = nullptr;
	ID3D11BlendState* m_blendState = nullptr;
	ID3D11SamplerState* m_samplerState = nullptr;
	ID3D11DepthStencilState* m_depthStencilState = nullptr;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	ID3D11Texture2D* m_depthStencilTexture = nullptr;
	void* m_dxgiDebugLibModule = nullptr;
	void* m_dxgiDebugInterface = nullptr;
};
