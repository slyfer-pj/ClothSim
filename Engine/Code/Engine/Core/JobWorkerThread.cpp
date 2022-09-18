#include "Engine/Core/JobWorkerThread.hpp"
#include "Engine/Core/Job.hpp"
#include "Engine/Core/JobSystem.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include <chrono>

JobWorkerThread::JobWorkerThread(JobSystem* jobSystem, int workerThreadID)
	:m_jobSystem(jobSystem), m_workerThreadID(workerThreadID)
{
	m_thread = new std::thread(&JobWorkerThread::JobWorkerMain, this, workerThreadID);
}

JobWorkerThread::~JobWorkerThread()
{
	m_thread->join();
	delete m_thread;
	m_thread = nullptr;
}

void JobWorkerThread::JobWorkerMain(int workerID)
{
	UNUSED(workerID);
	while (!m_isQuitting)
	{
		Job* jobToExecute = m_jobSystem->ClaimJobToExecute();
		if (jobToExecute)
		{
			jobToExecute->Execute();
			m_jobSystem->MoveJobToFinishedQueue(jobToExecute);
			jobToExecute->OnFinished();
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::microseconds(10));
		}
	}
}

