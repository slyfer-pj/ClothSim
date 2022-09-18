#include "Engine/Core/Clock.hpp"
#include "Engine/Core/Time.hpp"

static Clock g_systemClock;

Clock::Clock()
{
	if (this != &g_systemClock)
	{
		SetParent(g_systemClock);
		g_systemClock.AddChild(this);
	}
}

Clock::Clock(Clock& parent)
{
	SetParent(parent);
	parent.AddChild(this);
}

void Clock::SetParent(Clock& newParent)
{
	if (m_parent != nullptr)
	{
		m_parent->RemoveChild(this);
	}
	m_parent = &newParent;
}

void Clock::Pause()
{
	m_isPaused = true;
}

void Clock::Unpause()
{
	m_isPaused = false;
	m_pauseAfterFrame = false;
}

void Clock::TogglePause()
{
	m_isPaused = !m_isPaused;
	m_pauseAfterFrame = false;
}

void Clock::StepFrame()
{
	m_isPaused = false;
	m_pauseAfterFrame = true;
}

void Clock::SetTimeDilation(double dilationAmount)
{
	m_timeDilation = dilationAmount;
}

double Clock::GetDeltaTime() const
{
	return m_deltaTime;
}

double Clock::GetTotalTime() const
{
	return m_totalTime;
}

size_t Clock::GetFrameCount() const
{
	return m_frameCount;
}

bool Clock::IsPaused() const
{
	return m_isPaused;
}

double Clock::GetTimeDilation() const
{
	return m_timeDilation;
}

void Clock::SystemBeginFrame()
{
	g_systemClock.Tick();
}

Clock& Clock::GetSystemClock()
{
	return g_systemClock;
}

void Clock::Tick()
{
	double currentTime = GetCurrentTimeSeconds();
	double deltaSeconds = currentTime - m_lastUpdateTime;
	if (deltaSeconds > 0.1)
		deltaSeconds = 0.1;

	m_lastUpdateTime = currentTime;
	Advance(deltaSeconds);
}

void Clock::Advance(double deltaTimeSeconds)
{
	if (m_pauseAfterFrame)
		deltaTimeSeconds = 0.016;
	if (m_isPaused)
		deltaTimeSeconds *= 0.0;

	deltaTimeSeconds *= m_timeDilation;

	m_deltaTime = deltaTimeSeconds;
	m_totalTime += deltaTimeSeconds;
	m_frameCount++;

	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] != nullptr)
		{
			//m_children[i]->SetTimeDilation(m_timeDilation);

			/*if (m_isPaused)
				m_children[i]->Pause();
			else
				m_children[i]->Unpause();*/

			m_children[i]->Advance(m_deltaTime);
		}
	}

	if (m_pauseAfterFrame)
		m_isPaused = true;
}

void Clock::AddChild(Clock* childClock)
{
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] == nullptr)
		{
			m_children[i] = childClock;
			return;
		}
	}
	m_children.push_back(childClock);
}

void Clock::RemoveChild(Clock* childClock)
{
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] == childClock)
		{
			m_children[i] = nullptr;
		}
	}
}

Clock::~Clock()
{
	if(m_parent)
		m_parent->RemoveChild(this);
	
	//set parent of all children to system clock
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] != nullptr)
		{
			m_children[i]->SetParent(g_systemClock);
		}
	}
}
