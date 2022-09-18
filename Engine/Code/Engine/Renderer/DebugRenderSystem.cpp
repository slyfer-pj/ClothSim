#include "Engine/Renderer/DebugRenderSystem.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/DebugRenderObject.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"
#include <mutex>

constexpr float cellHeight = 25.f;
extern Renderer* g_theRenderer;

class DebugRenderSystem
{
public:
	DebugRenderSystem(DebugRenderSystemConfig config);
	void AddToList(std::vector<DebugRenderObject*>& list, DebugRenderObject& obj, std::mutex& listMutex);
	void AddToListAccordingToMode(DebugRenderMode mode, DebugRenderObject& obj);
	void RemoveFromList(std::vector<DebugRenderObject*>& list, DebugRenderObject& obj, std::mutex& listMutex);

public:
	DebugRenderSystemConfig m_config;
	Clock m_debugRenderClock;
	bool m_showDebugInfo = !m_config.m_startHidden;
	std::vector<DebugRenderObject*> m_allDebugObject;
	std::mutex m_allObjectListMutex;
	std::vector<DebugRenderObject*> m_depthModeObjects;
	std::mutex m_depthObjectsMutex;
	std::vector<DebugRenderObject*> m_alwaysModeObjects;
	std::mutex m_alwaysModeObjectsMutex;
	std::vector<DebugRenderObject*> m_xrayModeObjects;
	std::mutex m_xrayModeObjectsMutex;
	std::vector<DebugRenderObject*> m_worldTextObjects;
	std::mutex m_worldTextObjectMutex;
	std::vector<DebugRenderObject*> m_textObjects;
	std::mutex m_textObjectMutex;
	std::vector<DebugRenderObject*> m_debugMessages;
	std::mutex m_debugMessagesMutex;
	BitmapFont* m_font = nullptr;
	IntVec2 m_uiScreenSize = IntVec2::ZERO;
	Camera m_worldCamera;
};

DebugRenderSystem* g_debugRenderSystem = nullptr;

DebugRenderSystem::DebugRenderSystem(DebugRenderSystemConfig config)
	:m_config(config)
{
	m_font = m_config.m_renderer->CreateOrGetBitmapFont(config.m_fontFilepath.c_str());
	m_uiScreenSize = IntVec2(g_gameConfigBlackboard.GetValue("screenSizeWidth", m_uiScreenSize.x), g_gameConfigBlackboard.GetValue("screenSizeHeight", m_uiScreenSize.x));
}

void DebugRenderSystem::AddToList(std::vector<DebugRenderObject*>& list, DebugRenderObject& obj, std::mutex& listMutex)
{
	/*std::vector<DebugRenderObject*>& listToAddTo = m_allDebugObject;
	if (list == m_depthModeObjects)
		listToAddTo = m_depthModeObjects;
	else if (list == m_alwaysModeObjects)
		listToAddTo = m_alwaysModeObjects;
	else if (list == m_xrayModeObjects)
		listToAddTo = m_xrayModeObjects;
	else if (list == m_textObjects)
		listToAddTo = m_textObjects;*/

	listMutex.lock();
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i] == nullptr)
		{
			list[i] = &obj;
			listMutex.unlock();
			return;
		}
	}

	list.push_back(&obj);
	listMutex.unlock();
}

void DebugRenderSystem::AddToListAccordingToMode(DebugRenderMode mode, DebugRenderObject& obj)
{
	switch (mode)
	{
	case DebugRenderMode::ALWAYS:
		g_debugRenderSystem->AddToList(m_alwaysModeObjects, obj, m_alwaysModeObjectsMutex);
		break;
	case DebugRenderMode::USEDEPTH:
		g_debugRenderSystem->AddToList(m_depthModeObjects, obj, m_depthObjectsMutex);
		break;
	case DebugRenderMode::XRAY:
		g_debugRenderSystem->AddToList(m_xrayModeObjects, obj, m_xrayModeObjectsMutex);
		break;
	default:
		break;
	}
}

void DebugRenderSystem::RemoveFromList(std::vector<DebugRenderObject*>& list, DebugRenderObject& obj, std::mutex& listMutex)
{
	listMutex.lock();
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i] == &obj)
			list[i] = nullptr;
	}
	listMutex.unlock();
}

void DebugAddWorldWireSphere(const Vec3& center, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	if (g_debugRenderSystem)
	{
		std::vector<Vertex_PCU> verts;
		AddVertsForSphere(verts, 16, 8, radius);
		DebugRenderObject* renderObj = new DebugRenderObject();
		renderObj->m_transform = Mat44::CreateTranslation3D(center);
		renderObj->m_totalLifeDuration = duration;
		renderObj->m_localVerts = verts;
		renderObj->m_startColor = startColor;
		renderObj->m_endColor = endColor;
		renderObj->m_debugRenderMode = mode;
		renderObj->m_fillMode = FillMode::WIREFRAME;
		renderObj->Startup(g_debugRenderSystem->m_debugRenderClock);
		g_debugRenderSystem->AddToList(g_debugRenderSystem->m_allDebugObject, *renderObj, g_debugRenderSystem->m_allObjectListMutex);
		g_debugRenderSystem->AddToListAccordingToMode(mode, *renderObj);
	}
}

void DebugAddWorldSphere(const Vec3& center, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	if (g_debugRenderSystem)
	{
		std::vector<Vertex_PCU> verts;
		AddVertsForSphere(verts, 16, 8, radius);
		DebugRenderObject* renderObj = new DebugRenderObject();
		renderObj->m_transform = Mat44::CreateTranslation3D(center);
		renderObj->m_totalLifeDuration = duration;
		renderObj->m_localVerts = verts;
		renderObj->m_startColor = startColor;
		renderObj->m_endColor = endColor;
		renderObj->m_debugRenderMode = mode;
		renderObj->m_fillMode = FillMode::SOLID;
		renderObj->Startup(g_debugRenderSystem->m_debugRenderClock);
		g_debugRenderSystem->AddToList(g_debugRenderSystem->m_allDebugObject, *renderObj, g_debugRenderSystem->m_allObjectListMutex);
		g_debugRenderSystem->AddToListAccordingToMode(mode, *renderObj);
	}
}

void DebugAddWorldArrow(const Vec3& start, const Vec3& end, float radius, float duration, const Rgba8& baseColor, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	UNUSED(baseColor);
	if (g_debugRenderSystem)
	{
		std::vector<Vertex_PCU> verts;
		AddVertsForArrow3D(verts, start, end, radius, Rgba8::WHITE);
		DebugRenderObject* renderObj = new DebugRenderObject();
		renderObj->m_totalLifeDuration = duration;
		renderObj->m_localVerts = verts;
		renderObj->m_startColor = startColor;
		renderObj->m_endColor = endColor;
		renderObj->m_debugRenderMode = mode;
		renderObj->m_fillMode = FillMode::SOLID;
		renderObj->Startup(g_debugRenderSystem->m_debugRenderClock);
		g_debugRenderSystem->AddToList(g_debugRenderSystem->m_allDebugObject, *renderObj, g_debugRenderSystem->m_allObjectListMutex);
		g_debugRenderSystem->AddToListAccordingToMode(mode, *renderObj);
	}
}

void DebugAddWorldBox(const AABB3& bounds, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	if (g_debugRenderSystem)
	{
		std::vector<Vertex_PCU> verts;
		AddVertsForAABB3D(verts, bounds, Rgba8::WHITE, AABB2(Vec2::ZERO, Vec2::ONE));
		DebugRenderObject* renderObj = new DebugRenderObject();
		renderObj->m_totalLifeDuration = duration;
		renderObj->m_localVerts = verts;
		renderObj->m_startColor = startColor;
		renderObj->m_endColor = endColor;
		renderObj->m_debugRenderMode = mode;
		renderObj->m_fillMode = FillMode::SOLID;
		renderObj->Startup(g_debugRenderSystem->m_debugRenderClock);
		g_debugRenderSystem->AddToList(g_debugRenderSystem->m_allDebugObject, *renderObj, g_debugRenderSystem->m_allObjectListMutex);
		g_debugRenderSystem->AddToListAccordingToMode(mode, *renderObj);
	}
}

void DebugAddWorldBasis(const Mat44& basis, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	UNUSED(startColor);
	UNUSED(endColor);
	if (g_debugRenderSystem)
	{
		Vec3 startPoint = basis.GetTranslation3D();
		DebugAddWorldArrow(startPoint, startPoint + basis.GetIBasis3D(), 0.2f, duration, Rgba8::RED, Rgba8::RED, Rgba8::RED, mode);
		DebugAddWorldArrow(startPoint, startPoint + basis.GetJBasis3D(), 0.2f, duration, Rgba8::GREEN, Rgba8::GREEN, Rgba8::GREEN, mode);
		DebugAddWorldArrow(startPoint, startPoint + basis.GetKBasis3D(), 0.2f, duration, Rgba8::BLUE, Rgba8::BLUE, Rgba8::BLUE, mode);
	}
}

void DebugAddWorldText(const std::string& text, const Mat44& transform, float textHeight, const Vec2& alignment, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	if (g_debugRenderSystem)
	{
		std::vector<Vertex_PCU>verts;
		Vec2 position = transform.GetTranslation2D();
		g_debugRenderSystem->m_font->AddVertsForText2D(verts, position, textHeight, text);
		DebugRenderObject* renderObj = new DebugRenderObject();
		renderObj->m_transform = transform;
		renderObj->m_totalLifeDuration = duration;
		renderObj->m_localVerts = verts;
		renderObj->m_startColor = startColor;
		renderObj->m_endColor = endColor;
		renderObj->m_debugRenderMode = mode;
		renderObj->m_fillMode = FillMode::SOLID;
		renderObj->m_worldTextObject = true;
		renderObj->m_billboardTextAlignment = alignment;
		renderObj->Startup(g_debugRenderSystem->m_debugRenderClock);
		g_debugRenderSystem->AddToList(g_debugRenderSystem->m_allDebugObject, *renderObj, g_debugRenderSystem->m_allObjectListMutex);
		g_debugRenderSystem->AddToList(g_debugRenderSystem->m_worldTextObjects, *renderObj, g_debugRenderSystem->m_worldTextObjectMutex);
	}
}

void DebugAddWorldBillboardText(const std::string& text, const Vec3& origin, float textHeight, const Vec2& alignment, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	if (g_debugRenderSystem)
	{
		std::vector<Vertex_PCU> verts;
		g_debugRenderSystem->m_font->AddVertsForText2D(verts, Vec2::ZERO, textHeight, text);
		float textWidth = g_debugRenderSystem->m_font->GetTextWidth(textHeight, text);
		DebugRenderObject* renderObj = new DebugRenderObject();
		renderObj->m_transform = Mat44::CreateTranslation3D(origin);
		//renderObj->m_transform.Append(Mat44::CreateTranslation2D(Vec2(-alignment.x * textWidth, -alignment.y * textWidth)));
		renderObj->m_totalLifeDuration = duration;
		renderObj->m_localVerts = verts;
		renderObj->m_startColor = startColor;
		renderObj->m_endColor = endColor;
		renderObj->m_debugRenderMode = mode;
		renderObj->m_fillMode = FillMode::SOLID;
		renderObj->m_worldTextObject = true;
		renderObj->m_isBillboard = true;
		renderObj->m_camera = &g_debugRenderSystem->m_worldCamera;
		renderObj->m_billboardTextAlignment = alignment;
		renderObj->m_billboardTextWidth = textWidth;
		renderObj->m_billboardTextHeight = textHeight;
		renderObj->Startup(g_debugRenderSystem->m_debugRenderClock);
		g_debugRenderSystem->AddToList(g_debugRenderSystem->m_allDebugObject, *renderObj, g_debugRenderSystem->m_allObjectListMutex);
		g_debugRenderSystem->AddToList(g_debugRenderSystem->m_worldTextObjects, *renderObj, g_debugRenderSystem->m_worldTextObjectMutex);
	}
}

void DebugAddScreenText(const std::string& text, const Vec2& position, float duration, const Vec2& alignment, float size, const Rgba8& startColor, const Rgba8& endColor)
{
	if (g_debugRenderSystem)
	{
		std::vector<Vertex_PCU> verts;
		g_debugRenderSystem->m_font->AddVertsForText2D(verts, position, size, text);
		DebugRenderObject* renderObj = new DebugRenderObject();
		renderObj->m_totalLifeDuration = duration;
		renderObj->m_localVerts = verts;
		renderObj->m_startColor = startColor;
		renderObj->m_endColor = endColor;
		renderObj->m_fillMode = FillMode::SOLID;
		renderObj->m_screenTextObject = true;
		renderObj->m_billboardTextAlignment = alignment;
		renderObj->Startup(g_debugRenderSystem->m_debugRenderClock);
		g_debugRenderSystem->AddToList(g_debugRenderSystem->m_allDebugObject, *renderObj, g_debugRenderSystem->m_allObjectListMutex);
		g_debugRenderSystem->AddToList(g_debugRenderSystem->m_textObjects, *renderObj, g_debugRenderSystem->m_textObjectMutex);
	}
}

void DebugAddMessage(const std::string& text, float duration, const Rgba8& startColor, const Rgba8& endColor)
{
	if (g_debugRenderSystem)
	{
		DebugRenderObject* renderObj = new DebugRenderObject();
		renderObj->m_totalLifeDuration = duration;
		renderObj->m_startColor = startColor;
		renderObj->m_endColor = endColor;
		renderObj->m_fillMode = FillMode::SOLID;
		renderObj->m_debugTextObject = true;
		renderObj->m_debugMessage = text;
		renderObj->m_debugMessage.append("\n");
		renderObj->Startup(g_debugRenderSystem->m_debugRenderClock);
		g_debugRenderSystem->AddToList(g_debugRenderSystem->m_allDebugObject, *renderObj, g_debugRenderSystem->m_allObjectListMutex);
		g_debugRenderSystem->AddToList(g_debugRenderSystem->m_debugMessages, *renderObj, g_debugRenderSystem->m_debugMessagesMutex);
	}
}

void DebugAddWorldWireCylinder(const Vec3& base, const Vec3& top, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	if (g_debugRenderSystem)
	{
		std::vector<Vertex_PCU> verts;
		AddVertsForCylinder3D(verts, base, top, radius, Rgba8::WHITE);
		DebugRenderObject* renderObj = new DebugRenderObject();
		renderObj->m_totalLifeDuration = duration;
		renderObj->m_localVerts = verts;
		renderObj->m_startColor = startColor;
		renderObj->m_endColor = endColor;
		renderObj->m_debugRenderMode = mode;
		renderObj->m_fillMode = FillMode::WIREFRAME;
		renderObj->Startup(g_debugRenderSystem->m_debugRenderClock);
		g_debugRenderSystem->AddToList(g_debugRenderSystem->m_allDebugObject, *renderObj, g_debugRenderSystem->m_allObjectListMutex);
		g_debugRenderSystem->AddToListAccordingToMode(mode, *renderObj);
	}
}

void DebugAddWorldPoint(const Vec3& pos, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	if (g_debugRenderSystem)
	{
		std::vector<Vertex_PCU> verts;
		AddVertsForSphere(verts, 16, 8, radius);
		DebugRenderObject* renderObj = new DebugRenderObject();
		renderObj->m_transform = Mat44::CreateTranslation3D(pos);
		renderObj->m_totalLifeDuration = duration;
		renderObj->m_localVerts = verts;
		renderObj->m_startColor = startColor;
		renderObj->m_endColor = endColor;
		renderObj->m_debugRenderMode = mode;
		renderObj->m_fillMode = FillMode::SOLID;
		renderObj->Startup(g_debugRenderSystem->m_debugRenderClock);
		g_debugRenderSystem->AddToList(g_debugRenderSystem->m_allDebugObject, *renderObj, g_debugRenderSystem->m_allObjectListMutex);
		g_debugRenderSystem->AddToListAccordingToMode(mode, *renderObj);
	}
}

void DebugAddWorldLine(const Vec3& start, const Vec3& end, float radius, float duration, const Rgba8& startColor, const Rgba8& endColor, DebugRenderMode mode)
{
	if (g_debugRenderSystem)
	{
		std::vector<Vertex_PCU> verts;
		AddVertsForCylinder3D(verts, start, end, radius, Rgba8::WHITE);
		DebugRenderObject* renderObj = new DebugRenderObject();
		renderObj->m_totalLifeDuration = duration;
		renderObj->m_localVerts = verts;
		renderObj->m_startColor = startColor;
		renderObj->m_endColor = endColor;
		renderObj->m_debugRenderMode = mode;
		renderObj->m_fillMode = FillMode::SOLID;
		renderObj->Startup(g_debugRenderSystem->m_debugRenderClock);
		g_debugRenderSystem->AddToList(g_debugRenderSystem->m_allDebugObject, *renderObj, g_debugRenderSystem->m_allObjectListMutex);
		g_debugRenderSystem->AddToListAccordingToMode(mode, *renderObj);
	}
}

void DebugRenderSystemStartup(const DebugRenderSystemConfig& config)
{
	g_debugRenderSystem = new DebugRenderSystem(config);
}

void DebugRenderSystemShutdown()
{
	DebugRenderClear();
	delete g_debugRenderSystem;
	g_debugRenderSystem = nullptr;
}

void DebugRenderSetVisible()
{
	g_debugRenderSystem->m_showDebugInfo = true;
}

void DebugRenderSetHidden()
{
	g_debugRenderSystem->m_showDebugInfo = false;
}

void DebugRenderToggleVisibility()
{
	g_debugRenderSystem->m_showDebugInfo = !g_debugRenderSystem->m_showDebugInfo;
}

void DebugRenderClear()
{
	g_debugRenderSystem->m_allObjectListMutex.lock();
	for (int i = 0; i < g_debugRenderSystem->m_allDebugObject.size(); i++)
	{
		if (g_debugRenderSystem->m_allDebugObject[i])
		{
			delete g_debugRenderSystem->m_allDebugObject[i];
			g_debugRenderSystem->m_allDebugObject[i] = nullptr;
		}
	}
	g_debugRenderSystem->m_allObjectListMutex.unlock();

	g_debugRenderSystem->m_alwaysModeObjects.clear();
	g_debugRenderSystem->m_xrayModeObjects.clear();
	g_debugRenderSystem->m_depthModeObjects.clear();
	g_debugRenderSystem->m_worldTextObjects.clear();
	g_debugRenderSystem->m_textObjects.clear();
	g_debugRenderSystem->m_debugMessages.clear();
}

void DebugRenderSetParentClock(Clock& parent)
{
	g_debugRenderSystem->m_debugRenderClock.SetParent(parent);
}

void DebugRenderChangeTimeDilation(float deltaValue)
{
	double newTimeDilation = g_debugRenderSystem->m_debugRenderClock.GetTimeDilation() + (double)deltaValue;
	newTimeDilation = Clamp(newTimeDilation, 0.1, 10.0);
	g_debugRenderSystem->m_debugRenderClock.SetTimeDilation(newTimeDilation);
}

void DebugRenderBeginFrame()
{
	g_debugRenderSystem->m_allObjectListMutex.lock();
	for (int i = 0; i < g_debugRenderSystem->m_allDebugObject.size(); i++)
	{
		if (g_debugRenderSystem->m_allDebugObject[i])
			g_debugRenderSystem->m_allDebugObject[i]->Update();
	}
	g_debugRenderSystem->m_allObjectListMutex.unlock();


	/*std::string clockInfo = Stringf("Debug System | Time: %.2f FPS: %.2f Scale: %.2f", g_debugRenderSystem->m_debugRenderClock.GetTotalTime(), 1.f / g_debugRenderSystem->m_debugRenderClock.GetDeltaTime(), g_debugRenderSystem->m_debugRenderClock.GetTimeDilation());
	DebugAddScreenText(clockInfo, Vec2(g_debugRenderSystem->m_uiScreenSize.x - 550.f, g_debugRenderSystem->m_uiScreenSize.y - 32.f), FLT_MIN, Vec2::ZERO, 10.f, Rgba8::WHITE, Rgba8::WHITE);*/
}

void DebugRenderWorld(const Camera& camera)
{
	if (g_debugRenderSystem)
	{
		g_debugRenderSystem->m_worldCamera = camera;

		if (!g_debugRenderSystem->m_showDebugInfo)
			return;

		g_theRenderer->BindShaderByName("Default");
		g_theRenderer->SetBlendMode(BlendMode::ALPHA);
		g_theRenderer->SetSamplerMode(SamplerMode::POINTCLAMP);
		g_theRenderer->SetDepthStencilState(DepthTest::ALWAYS, false);
		g_debugRenderSystem->m_alwaysModeObjectsMutex.lock();
		for (int i = 0; i < g_debugRenderSystem->m_alwaysModeObjects.size(); i++)
		{
			if (g_debugRenderSystem->m_alwaysModeObjects[i] == nullptr)
				continue;
			DebugRenderObject& obj = *(g_debugRenderSystem->m_alwaysModeObjects[i]);
			
			g_theRenderer->SetRasterizerState(CullMode::BACK, obj.m_fillMode, WindingOrder::COUNTERCLOCKWISE);
			g_theRenderer->SetModelMatrix(obj.GetModelMatrix());
			g_theRenderer->SetModelColor(obj.m_tint);
			g_theRenderer->BindTexture(nullptr);
			g_theRenderer->DrawVertexArray((int)obj.m_localVerts.size(), obj.m_localVerts.data());
		}
		g_debugRenderSystem->m_alwaysModeObjectsMutex.unlock();

		g_theRenderer->BindShaderByName("Default");
		g_theRenderer->SetBlendMode(BlendMode::ALPHA);
		g_theRenderer->SetSamplerMode(SamplerMode::POINTCLAMP);
		g_theRenderer->SetDepthStencilState(DepthTest::LESSEQUAL, true);
		g_debugRenderSystem->m_depthObjectsMutex.lock();
		for (int i = 0; i < g_debugRenderSystem->m_depthModeObjects.size(); i++)
		{
			if(g_debugRenderSystem->m_depthModeObjects[i] == nullptr)
				continue;
			DebugRenderObject& obj = *(g_debugRenderSystem->m_depthModeObjects[i]);
			
			g_theRenderer->SetRasterizerState(CullMode::BACK, obj.m_fillMode, WindingOrder::COUNTERCLOCKWISE);
			g_theRenderer->SetModelMatrix(obj.GetModelMatrix());
			g_theRenderer->SetModelColor(obj.m_tint);
			g_theRenderer->BindTexture(nullptr);
			g_theRenderer->DrawVertexArray((int)obj.m_localVerts.size(), obj.m_localVerts.data());
		}
		g_debugRenderSystem->m_depthObjectsMutex.unlock();


		g_debugRenderSystem->m_xrayModeObjectsMutex.lock();
		for (int i = 0; i < g_debugRenderSystem->m_xrayModeObjects.size(); i++)
		{
			if (g_debugRenderSystem->m_xrayModeObjects[i] == nullptr)
				continue;
			DebugRenderObject& obj = *(g_debugRenderSystem->m_xrayModeObjects[i]);
			g_theRenderer->BindShaderByName("Default");
			g_theRenderer->SetBlendMode(BlendMode::ALPHA);
			g_theRenderer->SetSamplerMode(SamplerMode::POINTCLAMP);
			g_theRenderer->SetRasterizerState(CullMode::BACK, obj.m_fillMode, WindingOrder::COUNTERCLOCKWISE);
			g_theRenderer->SetDepthStencilState(DepthTest::ALWAYS, false);
			g_theRenderer->SetModelMatrix(obj.GetModelMatrix());
			g_theRenderer->SetModelColor(Rgba8(unsigned char(obj.m_tint.r * 0.5f), unsigned char(obj.m_tint.g * 0.5f), unsigned char(obj.m_tint.b * 0.5f), unsigned char(obj.m_tint.a * 0.5f)));
			g_theRenderer->BindTexture(nullptr);
			g_theRenderer->DrawVertexArray((int)obj.m_localVerts.size(), obj.m_localVerts.data());

			g_theRenderer->SetBlendMode(BlendMode::ALPHA);
			g_theRenderer->SetSamplerMode(SamplerMode::POINTCLAMP);
			g_theRenderer->SetRasterizerState(CullMode::BACK, obj.m_fillMode, WindingOrder::COUNTERCLOCKWISE);
			g_theRenderer->SetDepthStencilState(DepthTest::LESSEQUAL, true);
			g_theRenderer->SetModelMatrix(obj.GetModelMatrix());
			g_theRenderer->SetModelColor(obj.m_tint);
			g_theRenderer->BindTexture(nullptr);
			g_theRenderer->DrawVertexArray((int)obj.m_localVerts.size(), obj.m_localVerts.data());
		}
		g_debugRenderSystem->m_xrayModeObjectsMutex.unlock();

		g_theRenderer->BindShaderByName("Default");
		g_theRenderer->SetBlendMode(BlendMode::ALPHA);
		g_theRenderer->SetSamplerMode(SamplerMode::POINTCLAMP);
		g_theRenderer->SetDepthStencilState(DepthTest::LESSEQUAL, true);
		g_debugRenderSystem->m_worldTextObjectMutex.lock();
		for (int i = 0; i < g_debugRenderSystem->m_worldTextObjects.size(); i++)
		{
			if (g_debugRenderSystem->m_worldTextObjects[i] == nullptr)
				continue;
			DebugRenderObject& obj = *(g_debugRenderSystem->m_worldTextObjects[i]);
			g_theRenderer->SetRasterizerState(CullMode::NONE, obj.m_fillMode, WindingOrder::COUNTERCLOCKWISE);
			g_theRenderer->SetModelMatrix(obj.GetModelMatrix());
			g_theRenderer->SetModelColor(obj.m_tint);
			g_theRenderer->BindTexture(&(g_debugRenderSystem->m_font->GetTexture()));
			g_theRenderer->DrawVertexArray((int)obj.m_localVerts.size(), obj.m_localVerts.data());
		}
		g_debugRenderSystem->m_worldTextObjectMutex.unlock();
	}
}

void DebugRenderScreen(const Camera& camera)
{
	if (g_debugRenderSystem)
	{
		UNUSED(camera);
		if (!g_debugRenderSystem->m_showDebugInfo)
			return;

		g_theRenderer->SetBlendMode(BlendMode::ALPHA);
		g_theRenderer->SetSamplerMode(SamplerMode::POINTCLAMP);
		g_theRenderer->SetRasterizerState(CullMode::NONE, FillMode::SOLID, WindingOrder::COUNTERCLOCKWISE);
		g_theRenderer->SetDepthStencilState(DepthTest::LESSEQUAL, true);
		g_debugRenderSystem->m_textObjectMutex.lock();
		for (int i = 0; i < g_debugRenderSystem->m_textObjects.size(); i++)
		{
			if (g_debugRenderSystem->m_textObjects[i] == nullptr)
				continue;
			DebugRenderObject& obj = *(g_debugRenderSystem->m_textObjects[i]);
			g_theRenderer->SetModelMatrix(obj.GetModelMatrix());
			g_theRenderer->SetModelColor(obj.m_tint);
			g_theRenderer->BindTexture(&(g_debugRenderSystem->m_font->GetTexture()));
			g_theRenderer->DrawVertexArray((int)obj.m_localVerts.size(), obj.m_localVerts.data());
		}
		g_debugRenderSystem->m_textObjectMutex.unlock();

		std::vector<Vertex_PCU> debugMessageVerts;
		std::string debugMessages;
		g_debugRenderSystem->m_debugMessagesMutex.lock();
		for (int i = 0; i < g_debugRenderSystem->m_debugMessages.size(); i++)
		{
			if (g_debugRenderSystem->m_debugMessages[i] == nullptr)
				continue;
			debugMessages.append(g_debugRenderSystem->m_debugMessages[i]->m_debugMessage);
		}
		g_debugRenderSystem->m_debugMessagesMutex.unlock();
		AABB2 bounds(Vec2(0.f, g_debugRenderSystem->m_uiScreenSize.y - 25.f), Vec2(25.f, g_debugRenderSystem->m_uiScreenSize.y - 15.f));
		g_debugRenderSystem->m_font->AddVertsForTextInBox2D(debugMessageVerts, bounds, 15.f, debugMessages, Rgba8::WHITE, 1.f, Vec2(0.f, 1.f), TextBoxMode::OVERRUN);
		g_theRenderer->SetBlendMode(BlendMode::ALPHA);
		g_theRenderer->SetSamplerMode(SamplerMode::POINTCLAMP);
		g_theRenderer->SetRasterizerState(CullMode::NONE, FillMode::SOLID, WindingOrder::COUNTERCLOCKWISE);
		g_theRenderer->SetDepthStencilState(DepthTest::LESSEQUAL, true);
		g_theRenderer->BindTexture(&(g_debugRenderSystem->m_font->GetTexture()));
		g_theRenderer->DrawVertexArray((int)debugMessageVerts.size(), debugMessageVerts.data());
	}
}

void DebugRenderEndFrame()
{
	g_debugRenderSystem->m_allObjectListMutex.lock();
	for (int i = 0; i < g_debugRenderSystem->m_allDebugObject.size(); i++)
	{
		if (g_debugRenderSystem->m_allDebugObject[i] && g_debugRenderSystem->m_allDebugObject[i]->m_isDead)
		{
			DebugRenderObject& obj = *(g_debugRenderSystem->m_allDebugObject[i]);

			if (obj.m_screenTextObject)
				g_debugRenderSystem->RemoveFromList(g_debugRenderSystem->m_textObjects, obj, g_debugRenderSystem->m_textObjectMutex);
			else if (obj.m_debugTextObject)
				g_debugRenderSystem->RemoveFromList(g_debugRenderSystem->m_debugMessages, obj, g_debugRenderSystem->m_debugMessagesMutex);
			else if (obj.m_worldTextObject)
				g_debugRenderSystem->RemoveFromList(g_debugRenderSystem->m_worldTextObjects, obj, g_debugRenderSystem->m_worldTextObjectMutex);
			else if (obj.m_debugRenderMode == DebugRenderMode::ALWAYS)
				g_debugRenderSystem->RemoveFromList(g_debugRenderSystem->m_alwaysModeObjects, obj, g_debugRenderSystem->m_alwaysModeObjectsMutex);
			else if (obj.m_debugRenderMode == DebugRenderMode::USEDEPTH)
				g_debugRenderSystem->RemoveFromList(g_debugRenderSystem->m_depthModeObjects, obj, g_debugRenderSystem->m_depthObjectsMutex);
			else if (obj.m_debugRenderMode == DebugRenderMode::XRAY)
				g_debugRenderSystem->RemoveFromList(g_debugRenderSystem->m_xrayModeObjects, obj, g_debugRenderSystem->m_xrayModeObjectsMutex);

			delete g_debugRenderSystem->m_allDebugObject[i];
			g_debugRenderSystem->m_allDebugObject[i] = nullptr;
		}
	}
	g_debugRenderSystem->m_allObjectListMutex.unlock();
}


