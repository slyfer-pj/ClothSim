#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/Window.hpp"
#include "ThirdParty/ImGUI/imgui.h"
#include "Game/Game.hpp"
#include "Game/App.hpp"
#include "Game/Cloth.hpp"
#include "Game/Plant.hpp"

extern App* g_theApp;
extern Renderer* g_theRenderer;
extern InputSystem* g_theInput;
extern AudioSystem* g_theAudio;
extern Window* g_theWindow;

static float animationTimer = 0.f;
constexpr float COLLISION_CIRCLE_RADIUS = 7.f;
constexpr float COLLISION_BOX_WIDTH = 15.f;
constexpr float COLLISION_BOX_HEIGHT = 10.f;
constexpr float COLLISION_OBJECT_MOVE_SPEED = 1.f;


void Game::Startup()
{
	m_attractMode = true;
	InitializeAttractScreenDrawVertices();
	m_uiScreenSize = g_gameConfigBlackboard.GetValue("screenSize", m_uiScreenSize);
	m_worldSize = g_gameConfigBlackboard.GetValue("worldSize", m_worldSize);
	m_worldCamera.SetOrthoView(Vec2(0.f, 0.f), m_worldSize);
	m_screenCamera.SetOrthoView(Vec2(0.f, 0.f), m_uiScreenSize);
	m_stopwatch.Start(&m_gameClock, 1.f);
	InitializeMode();
}

void Game::ShutDown()
{
	CleanupCurrentMode();
}

void Game::Update(float deltaSeconds)
{
	UNUSED(deltaSeconds);

	if (m_attractMode)
	{
		HandleAttractModeInput();
		UpdateAttractMode(deltaSeconds);
		//DebuggerPrintf("Stopwatch: elapsed duration = %f, elapsed fraction = %f \n", m_stopwatch.GetElapsedTime(), m_stopwatch.GetElapsedFraction());
	}
	else
	{
		HandleGameInput();
		UpdateMode(deltaSeconds);
		//UpdateModePhysics(deltaSeconds);
		UpdatePhysics(deltaSeconds);
		//DemoImGUIWindow();
	}
}

void Game::Render() const
{
	if (m_attractMode)
	{
		g_theRenderer->BeginCamera(m_screenCamera);
		RenderAttractScreen();
		g_theRenderer->EndCamera(m_screenCamera);
	}
	else
	{
		g_theRenderer->BeginCamera(m_worldCamera);
		RenderMode();
		g_theRenderer->EndCamera(m_worldCamera);

		g_theRenderer->BeginCamera(m_screenCamera);
		RenderDebugInfoText();
		g_theRenderer->EndCamera(m_screenCamera);
	}

	g_theRenderer->BeginCamera(m_screenCamera);
	g_theConsole->Render(AABB2(m_screenCamera.GetOrthoBottomLeft(), m_screenCamera.GetOrthoTopRight()));
	g_theRenderer->EndCamera(m_screenCamera);
}

void Game::ChangeGameMode()
{
	CleanupCurrentMode();
	int currentModeIndex = static_cast<int>(m_currentMode);
	currentModeIndex++;
	if (currentModeIndex >= static_cast<int>(NUM_MODES))
	{
		currentModeIndex = 0;
	}

	m_currentMode = static_cast<GameMode>(currentModeIndex);
	InitializeMode();
}

void Game::InitializeMode()
{
	switch (m_currentMode)
	{
	case GAME_MODE_CLOTH:
	{
		m_cloth = new Cloth(this, IntVec2(30, 15), Vec2(3.f, 3.f), ClothMassType::UNIFORM);
		m_collisionCirclePosition = Vec2(90.f, 10.f);
		Vec2 boxMins = Vec2(10.f, 90.f);
		m_collisionBox = AABB2(boxMins, boxMins + Vec2(COLLISION_BOX_WIDTH, COLLISION_BOX_HEIGHT));
		break;
	}
	case GAME_MODE_PLANT:
	{
		m_plant = new Plant(this, Vec2(100.f, 20.f));
		m_plant2 = new Plant(this, Vec2(50.f, 30.f));
		break;
	}
	case NUM_MODES:
	{
		ERROR_AND_DIE("Invalid mode");
		break;
	}
	default:
	{
		ERROR_AND_DIE("Invalid mode");
		break;
	}
	}
}

void Game::CleanupCurrentMode()
{
	switch (m_currentMode)
	{
	case GAME_MODE_CLOTH:
	{
		delete m_cloth;
		m_cloth = nullptr;
		break;
	}
	case GAME_MODE_PLANT:
	{
		delete m_plant;
		m_plant = nullptr;
		delete m_plant2;
		m_plant2 = nullptr;
		break;
	}
	case NUM_MODES:
	{
		ERROR_AND_DIE("Invalid mode");
		break;
	}
	default:
	{
		ERROR_AND_DIE("Invalid mode");
		break;
	}
	}
}

void Game::UpdateMode(float deltaSeconds)
{
	UNUSED(deltaSeconds);
	switch (m_currentMode)
	{
	case GAME_MODE_CLOTH:
	{
		ClothControlPanel();
		break;
	}
	case GAME_MODE_PLANT:
	{
		break;
	}
	case NUM_MODES:
	{
		ERROR_AND_DIE("Invalid mode");
		break;
	}
	default:
	{
		ERROR_AND_DIE("Invalid mode");
		break;
	}
	}
}

void Game::UpdateModePhysics(float deltaSeconds)
{
	switch (m_currentMode)
	{
	case GAME_MODE_CLOTH:
	{
		if (m_moveParticle)
			m_cloth->MovePoint(m_screenMousePos, m_grabbedClothPoint);
		m_cloth->Update(deltaSeconds);
		m_cloth->CollideWithCircle(m_collisionCirclePosition, COLLISION_CIRCLE_RADIUS);
		m_cloth->CollideWithBox(m_collisionBox);
		break;
	}
	case GAME_MODE_PLANT:
	{
		if (m_moveParticle)
			m_plant->MovePoint(m_screenMousePos, m_grabbedPlantPoint);
		m_plant->Update(deltaSeconds);
		m_plant2->Update(deltaSeconds);
		break;
	}
	case NUM_MODES:
	{
		ERROR_AND_DIE("Invalid mode");
		break;
	}
	default:
	{
		ERROR_AND_DIE("Invalid mode");
		break;
	}
	}
}

void Game::UpdatePhysics(float deltaSeconds)
{
	m_physicsTimeOwed += deltaSeconds;
	while (m_physicsTimeOwed > m_physicsFixedTimeStep)
	{
		UpdateModePhysics(m_physicsFixedTimeStep);
		m_physicsTimeOwed -= m_physicsFixedTimeStep;
	}
}

void Game::RenderMode() const
{
	switch (m_currentMode)
	{
	case GAME_MODE_CLOTH:
	{
		RenderCollisionBox();
		RenderCollisionCircle();
		m_cloth->Render();
		break;
	}
	case GAME_MODE_PLANT:
	{
		m_plant->Render();
		m_plant2->Render();
		break;
	}
	case NUM_MODES:
	{
		ERROR_AND_DIE("Invalid mode");
		break;
	}
	default:
	{
		ERROR_AND_DIE("Invalid mode");
		break;
	}
	}
}

void Game::HandleAttractModeInput()
{
	const XboxController& controller = g_theInput->GetController(0);

	if (g_theInput->WasKeyJustPressed(KEYCODE_ESCAPE))
		g_theApp->HandleQuitRequest();
	if (g_theInput->WasKeyJustPressed(' ') || controller.WasButtonJustPressed(XBOX_BUTTON_START))
		m_attractMode = false;
}

void Game::HandleGameInput()
{
	const XboxController& controller = g_theInput->GetController(0);

	if (g_theInput->WasKeyJustPressed(' ') || controller.WasButtonJustPressed(XBOX_BUTTON_A))
	{
		SoundID testSound = g_theAudio->CreateOrGetSound("Data/Audio/TestSound.mp3");
		g_theAudio->StartSound(testSound);
	}
	if (g_theInput->WasKeyJustPressed(KEYCODE_ESCAPE))
	{
		m_attractMode = true;
		m_stopwatch.Restart();
	}
	if (g_theInput->WasKeyJustPressed(KEYCODE_F8))
	{
		ChangeGameMode();
	}
	if (g_theInput->IsKeyDown(KEYCODE_LEFT_MOUSE) || g_theInput->WasKeyJustPressed(KEYCODE_LEFT_MOUSE))
	{
		Vec2 normalizedMousePosition = g_theWindow->GetNormalizedCursorPos();
		m_screenMousePos = m_worldSize * normalizedMousePosition;
		m_moveParticle = true;
		/*if(m_cloth)
			m_cloth->MovePoint(screenMousePosition, m_grabbedClothPoint);
		if(m_plant)
			m_plant->MovePoint(screenMousePosition, m_grabbedPlantPoint);*/
	}
	if (g_theInput->WasKeyJustReleased(KEYCODE_LEFT_MOUSE))
	{
		m_moveParticle = false;
		m_grabbedClothPoint = nullptr;
		m_grabbedPlantPoint = nullptr;
	}
	if (g_theInput->WasKeyJustPressed(KEYCODE_F1))
	{
		m_debugRender = !m_debugRender;
	}
	if (g_theInput->WasKeyJustPressed('B') && m_grabbedClothPoint)
	{
		m_grabbedClothPoint->m_isPinned = !m_grabbedClothPoint->m_isPinned;
	}
	if (g_theInput->IsKeyDown('C') && m_grabbedClothPoint)
	{
		m_cloth->BreakConstraintsWithNeighbours(m_grabbedClothPoint);
	}
	if (g_theInput->WasKeyJustPressed(KEYCODE_F2))
	{
		m_renderClothGrid = !m_renderClothGrid;
	}
	if (g_theInput->WasKeyJustPressed(KEYCODE_F3))
	{
		m_renderClothTexture = !m_renderClothTexture;
	}
	if (g_theInput->IsKeyDown(KEYCODE_F4))
	{
		if (m_cloth)
			m_cloth->ChangeHorizontalForceBy(10.f);
		if(m_plant)
			m_plant->ChangeHorizontalForceBy(10.f);
		if (m_plant2)
			m_plant2->ChangeHorizontalForceBy(10.f);
	}
	if (g_theInput->IsKeyDown(KEYCODE_F5))
	{
		if (m_cloth)
			m_cloth->ChangeHorizontalForceBy(-10.f);
		if (m_plant)
			m_plant->ChangeHorizontalForceBy(-10.f);
		if (m_plant2)
			m_plant2->ChangeHorizontalForceBy(-10.f);
	}
	if (g_theInput->WasKeyJustPressed('1'))
	{
		m_plant->m_renderStructureOnly = !m_plant->m_renderStructureOnly;
		m_plant2->m_renderStructureOnly = !m_plant2->m_renderStructureOnly;
	}
	if (g_theInput->IsKeyDown(KEYCODE_LEFTARROW))
	{
		m_collisionCirclePosition += Vec2(-1.f, 0.f);
	}
	if (g_theInput->IsKeyDown(KEYCODE_RIGHTARROW))
	{
		m_collisionCirclePosition += Vec2(1.f, 0.f);
	}
	if (g_theInput->IsKeyDown(KEYCODE_UPARROW))
	{
		m_collisionCirclePosition += Vec2(0.f, 1.f);
	}
	if (g_theInput->IsKeyDown(KEYCODE_DOWNARROW))
	{
		m_collisionCirclePosition += Vec2(0.f, -1.f);
	}
	if (g_theInput->IsKeyDown('W'))
	{
		m_collisionBox.Translate(Vec2(0.f, 1.f) * COLLISION_OBJECT_MOVE_SPEED);
	}
	if (g_theInput->IsKeyDown('A'))
	{
		m_collisionBox.Translate(Vec2(-1.f, 0.f) * COLLISION_OBJECT_MOVE_SPEED);
	}
	if (g_theInput->IsKeyDown('S'))
	{
		m_collisionBox.Translate(Vec2(0.f, -1.f) * COLLISION_OBJECT_MOVE_SPEED);
	}
	if (g_theInput->IsKeyDown('D'))
	{
		m_collisionBox.Translate(Vec2(1.f, 0.f) * COLLISION_OBJECT_MOVE_SPEED);
	}
}

void Game::InitializeAttractScreenDrawVertices()
{
	m_attractScreenDrawVertices[0].m_position = Vec3(-2.f, -2.f, 0.f);
	m_attractScreenDrawVertices[1].m_position = Vec3(0.f, 2.f, 0.f);
	m_attractScreenDrawVertices[2].m_position = Vec3(2.f, -2.f, 0.f);

	m_attractScreenDrawVertices[0].m_color = Rgba8(0, 255, 0, 255);
	m_attractScreenDrawVertices[1].m_color = Rgba8(0, 255, 0, 255);
	m_attractScreenDrawVertices[2].m_color = Rgba8(0, 255, 0, 255);
}

void Game::RenderAttractScreen() const
{
	Vertex_PCU tempCopyOfBlinkingTriangle[3];

	for (int i = 0; i < 3; i++)
	{
		tempCopyOfBlinkingTriangle[i].m_position = m_attractScreenDrawVertices[i].m_position;
		tempCopyOfBlinkingTriangle[i].m_color.r = m_attractScreenDrawVertices[i].m_color.r;
		tempCopyOfBlinkingTriangle[i].m_color.g = m_attractScreenDrawVertices[i].m_color.g;
		tempCopyOfBlinkingTriangle[i].m_color.b = m_attractScreenDrawVertices[i].m_color.b;
		tempCopyOfBlinkingTriangle[i].m_color.a = m_attractModeTriangleAlpha;
	}

	TransformVertexArrayXY3D(3, tempCopyOfBlinkingTriangle, 25.f, 0.f, m_uiScreenSize * 0.5f);
	/*std::vector<Vertex_PCU> verts;
	AddVertsForAABB2D(verts, AABB2(Vec2(SCREEN_SIZE_X * 0.25, SCREEN_SIZE_Y * 0.25), Vec2(SCREEN_SIZE_X * 0.75f, SCREEN_SIZE_Y * 0.75f)), Rgba8::GREEN);*/
	g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	/*g_theRenderer->SetRasterizerState(CullMode::NONE, FillMode::SOLID, WindingOrder::COUNTERCLOCKWISE);
	g_theRenderer->SetDepthStencilState(DepthTest::ALWAYS, false);*/
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->DrawVertexArray(3, tempCopyOfBlinkingTriangle);
	//g_theRenderer->DrawVertexArray(verts.size(), verts.data());
}

void Game::UpdateAttractMode(float deltaSeconds)
{
	UNUSED(deltaSeconds);

	if (m_stopwatch.CheckDurationElapsedAndDecrement())
	{
		float temp = m_attractTriangleMinAlpha;
		m_attractTriangleMinAlpha = m_attractTriangleMaxAlpha;
		m_attractTriangleMaxAlpha = temp;
	}
	float alpha = Interpolate(m_attractTriangleMinAlpha, m_attractTriangleMaxAlpha, m_stopwatch.GetElapsedFraction());
	m_attractModeTriangleAlpha = (unsigned char)alpha;
}

void Game::RenderQuad() const
{
	std::vector<Vertex_PCU> verts;
	AddVertsForAABB2D(verts, AABB2(m_worldSize * 0.25f, m_worldSize * 0.75f), Rgba8::RED);
	g_theRenderer->SetRasterizerState(CullMode::NONE, FillMode::SOLID, WindingOrder::COUNTERCLOCKWISE);
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->DrawVertexArray((int)verts.size(), verts.data());
}

void Game::RenderDebugInfoText() const
{
	constexpr float cellHeight = 15.f;
	std::vector<Vertex_PCU> verts;
	std::string fontFilepath = g_gameConfigBlackboard.GetValue("devconsoleDefaultFont", "");
	BitmapFont* font = g_theRenderer->CreateOrGetBitmapFont(fontFilepath.c_str());
	AABB2 textBox = AABB2(Vec2(0.f, m_uiScreenSize.y - cellHeight), m_uiScreenSize);

	float horizontalForce = 0.f;
	if (m_cloth)
		horizontalForce = m_cloth->GetCurrentHorizontalForce();
	else
		horizontalForce = m_plant->GetCurrentHorizontalForce();
	std::string text = Stringf("Horizontal Force = %.1f, FramesMS = %.1f (%.1f fps)", horizontalForce, m_gameClock.GetDeltaTime() * 1000.f, 1.f / m_gameClock.GetDeltaTime());

	font->AddVertsForTextInBox2D(verts, textBox, cellHeight, text, Rgba8::WHITE, 1.f, Vec2::ZERO);
	g_theRenderer->BindTexture(&font->GetTexture());
	g_theRenderer->DrawVertexArray((int)verts.size(), verts.data());
}

void Game::RenderCollisionCircle() const
{
	constexpr float ringThickness = 0.3f;
	std::vector<Vertex_PCU> verts;
	AddVertsForRing2D(verts, m_collisionCirclePosition, COLLISION_CIRCLE_RADIUS, Rgba8::YELLOW, ringThickness);
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->DrawVertexArray((int)verts.size(), verts.data());
}

void Game::RenderCollisionBox() const
{
	std::vector<Vertex_PCU> verts;
	AddVertsForAABB2D(verts, m_collisionBox, Rgba8::YELLOW);
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->DrawVertexArray((int)verts.size(), verts.data());
}

void Game::DemoImGUIWindow()
{
	bool show_demo_window = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}

void Game::ClothControlPanel()
{
	static int gridCoordsArray[2] = {};
	static float linkLength[2] = {};
	ImGui::Begin("Control Panel");
	ImGui::InputInt2("Dimensions", gridCoordsArray);
	ImGui::InputFloat2("X/Y Link Length", linkLength);
	if (ImGui::Button("Regenerate Cloth"))
	{
		if (m_cloth)
		{
			delete m_cloth;
			m_cloth = nullptr;
		}

		m_cloth = new Cloth(this, IntVec2(gridCoordsArray[0], gridCoordsArray[1]), Vec2(linkLength[0], linkLength[1]), ClothMassType::UNIFORM);
	}
	ImGui::End();
}

