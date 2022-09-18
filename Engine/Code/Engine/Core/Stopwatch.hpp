#pragma once

class Clock;

class Stopwatch
{
public:
	Stopwatch();
	explicit Stopwatch(double duration);
	Stopwatch(const Clock* clock, double duration);

	void Start(double duration);
	void Start(const Clock* clock, double duration);

	void Restart();
	void Stop();
	void Pause();
	void Resume();

	double GetElapsedTime() const;
	float GetElapsedFraction() const;
	bool IsStopped() const;
	bool IsPaused() const;
	bool HasDurationElapsed() const;

	bool CheckDurationElapsedAndDecrement();

private:
	const Clock* m_clock = nullptr;
	double m_startTime = 0.0;
	double m_duration = 0.0;
	double m_pauseStartTime = 0.0;
	double m_pauseTotalDuration = 0.0;
};
