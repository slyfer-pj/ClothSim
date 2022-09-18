#pragma once
#include "Engine/Math/Vec3.hpp"
#include "Engine/Core/Stopwatch.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Renderer/DebugRenderSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include <vector>
#include <string>

class Clock;
class BitmapFont;
class Camera;

class DebugRenderObject
{
public:
	void Startup(Clock& parentClock);
	void Update();
	//void Render() const;
	Mat44 GetModelMatrix() const;
	void InterpolateColor();

public:
	std::vector<Vertex_PCU> m_localVerts;
	bool m_screenTextObject = false;
	bool m_worldTextObject = false;
	bool m_debugTextObject = false;
	bool m_isBillboard = false;
	Vec2 m_billboardTextAlignment;
	float m_billboardTextWidth = 0.f;
	float m_billboardTextHeight = 0.f;
	Camera* m_camera = nullptr;
	std::string m_debugMessage;
	Mat44 m_transform;
	Mat44 m_billboardTransform;
	float m_totalLifeDuration = 0.f;
	Stopwatch m_stopwatch;
	bool m_isDead = false;
	Rgba8 m_tint;
	Rgba8 m_startColor;
	Rgba8 m_endColor;
	BitmapFont* m_font = nullptr;
	DebugRenderMode m_debugRenderMode = DebugRenderMode::ALWAYS;
	FillMode m_fillMode = FillMode::SOLID;
};