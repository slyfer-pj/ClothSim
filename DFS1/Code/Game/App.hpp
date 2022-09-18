#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Renderer/Camera.hpp"

class AttractScreen;
class Game;
class App
{
public:
	App() {}
	~App() {}
	void Startup();
	void Run();
	void Shutdown();
	void HandleQuitRequest();

	bool IsQuitting() const { return m_isQuitting; }
	bool IsPaused() const { return m_isPaused; }
	bool IsSlomo() const { return m_isSlomo; }

private:
	bool m_isQuitting = false;
	bool m_isPaused = false;
	bool m_isSlomo = false;
	bool m_stepMode = false;
	
	Game* m_theGame = nullptr;
	float m_timeAtPreviousFrame = 0.f;

private:
	void BeginFrame();
	void RunFrame();
	void Update();
	void Render() const;
	void EndFrame();
	void HandleKeyboardInput();
};