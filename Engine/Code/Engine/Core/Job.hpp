#pragma once
#include "Engine/Core/JobWorkerThread.hpp"

class Job
{
	friend class JobWorkerThread;
public:
	virtual ~Job() = default;

private:
	virtual void Execute() = 0;
	virtual void OnFinished() = 0;
};