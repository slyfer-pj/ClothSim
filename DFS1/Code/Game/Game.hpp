#pragma once
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/Stopwatch.hpp"
#include "Engine/Core/Clock.hpp"

constexpr float PHYSICS_FIXED_TIMESTEP = 0.01f;

class Cloth;
struct Particle;
class Plant;

enum GameMode
{
	GAME_MODE_CLOTH,
	GAME_MODE_PLANT,
	NUM_MODES
};

class Game 
{
public:
	Game() {};
	void Startup();
	void Update(float deltaSeconds);
	void Render() const;
	void ShutDown();

public:
	Particle* m_grabbedClothPoint = nullptr;
	Particle* m_grabbedPlantPoint = nullptr;
	bool m_debugRender = false;
	bool m_renderClothGrid = true;
	bool m_renderClothTexture = true;
	Vec2 m_worldSize = Vec2::ZERO;
	GameMode m_currentMode = GAME_MODE_CLOTH;

private:
	Vertex_PCU m_attractScreenDrawVertices[3];
	bool m_attractMode = false;
	unsigned char m_attractModeTriangleAlpha = 0;
	Camera m_worldCamera;
	Camera m_screenCamera;
	Clock m_gameClock;
	Stopwatch m_stopwatch;
	float m_attractTriangleMinAlpha = 50.f;
	float m_attractTriangleMaxAlpha = 255.f;
	Vec2 m_uiScreenSize = Vec2::ZERO;
	Cloth* m_cloth = nullptr;
	Plant* m_plant = nullptr;
	Plant* m_plant2 = nullptr;
	Vec2 m_collisionCirclePosition = Vec2::ZERO;
	AABB2 m_collisionBox = AABB2::ZERO_TO_ONE;
	float m_physicsTimeOwed = 0.f;
	float m_physicsFixedTimeStep = PHYSICS_FIXED_TIMESTEP;
	Vec2 m_screenMousePos = Vec2::ZERO;
	bool m_moveParticle = false;

private:
	void ChangeGameMode();
	void InitializeMode();
	void CleanupCurrentMode();
	void UpdateMode(float deltaSeconds);
	void UpdateModePhysics(float deltaSeconds);
	void UpdatePhysics(float deltaSeconds);
	void RenderMode() const;
	void HandleAttractModeInput();
	void HandleGameInput();
	void InitializeAttractScreenDrawVertices();
	void RenderAttractScreen() const;
	void UpdateAttractMode(float deltaSeconds);
	void RenderQuad() const;
	void RenderDebugInfoText() const;
	void RenderCollisionCircle() const;
	void RenderCollisionBox() const;
	void DemoImGUIWindow();
	void ClothControlPanel();
};