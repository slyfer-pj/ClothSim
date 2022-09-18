#include "Engine/Core/Stopwatch.hpp"
#include "Engine/Core/Clock.hpp"

Stopwatch::Stopwatch()
{
	m_clock = &Clock::GetSystemClock();
}

Stopwatch::Stopwatch(double duration)
	:m_duration(duration)
{
	Start(&Clock::GetSystemClock(), m_duration);
}

Stopwatch::Stopwatch(const Clock* clock, double duration)
	:m_clock(clock), m_duration(duration)
{
	Start(m_duration);
}

void Stopwatch::Start(double duration)
{
	m_duration = duration;
	m_startTime = m_clock->GetTotalTime();
}

void Stopwatch::Start(const Clock* clock, double duration)
{
	m_clock = clock;
	m_duration = duration;
	m_startTime = m_clock->GetTotalTime();
}

void Stopwatch::Restart()
{
	m_startTime = m_clock->GetTotalTime();
}

void Stopwatch::Stop()
{
	m_duration = 0.0;
}

void Stopwatch::Pause()
{
	m_pauseStartTime = m_clock->GetTotalTime();
}

void Stopwatch::Resume()
{
	m_pauseTotalDuration = m_clock->GetTotalTime() - m_pauseStartTime;
	m_pauseStartTime = 0.0;
}

double Stopwatch::GetElapsedTime() const
{
	if (IsStopped())
		return 0.0f;
	if (IsPaused())
	{
		double currentTime = m_clock->GetTotalTime();
		return (currentTime - m_startTime - (m_pauseTotalDuration + (currentTime - m_pauseStartTime)));
	}

	return m_clock->GetTotalTime() - m_startTime - m_pauseTotalDuration;
}

float Stopwatch::GetElapsedFraction() const
{
	double elapsedTime = GetElapsedTime();
	if (m_duration <= 0.0)
		return 0.f;
	return static_cast<float>(elapsedTime / m_duration);
}

bool Stopwatch::IsStopped() const
{
	return (m_duration <= 0.0);
}

bool Stopwatch::IsPaused() const
{
	return (m_pauseStartTime > 0.0);
}

bool Stopwatch::HasDurationElapsed() const
{
	return (GetElapsedTime() > m_duration);
}

bool Stopwatch::CheckDurationElapsedAndDecrement()
{
	if (IsStopped())
		return false;
	if (GetElapsedFraction() < 1.0f)
		return false;

	m_startTime += m_duration;
	return true;
}
