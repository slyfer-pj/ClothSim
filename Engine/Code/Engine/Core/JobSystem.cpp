#include "Engine/Core/JobSystem.hpp"
#include "Engine/Core/JobWorkerThread.hpp"

JobSystem::JobSystem(const JobSystemConfig& config)
	:m_config(config)
{
}

void JobSystem::Startup()
{
	m_workerThreads.reserve(m_config.m_numWorkerThreads);
	for (int i = 0; i < m_config.m_numWorkerThreads; i++)
	{
		JobWorkerThread* workerThread = new JobWorkerThread(this, i);
		m_workerThreads.push_back(workerThread);
	}
}

void JobSystem::Shutdown()
{
	DestroyAllThreads();
}

void JobSystem::BeginFrame()
{

}

void JobSystem::EndFrame()
{

}

void JobSystem::QueueJobs(Job* jobToExecute)
{
	m_queuedJobsMutex.lock();
	m_queuedJobs.push_back(jobToExecute);
	m_queuedJobsMutex.unlock();
}

Job* JobSystem::ClaimJobToExecute()
{
	m_queuedJobsMutex.lock();
	Job* jobToExecute = nullptr;
	if (!m_queuedJobs.empty())
	{
		jobToExecute = m_queuedJobs.front();
		m_queuedJobs.pop_front();
		m_executingJobsMutex.lock();
		m_executingJobs.push_back(jobToExecute);
		m_executingJobsMutex.unlock();
	}
	m_queuedJobsMutex.unlock();
	return jobToExecute;
}

void JobSystem::MoveJobToFinishedQueue(Job* job)
{
	m_finishedJobsMutex.lock();
	m_finishedJobs.push_back(job);
	m_finishedJobsMutex.unlock();

	m_executingJobsMutex.lock();
	for (auto iter = m_executingJobs.begin(); iter != m_executingJobs.end(); ++iter)
	{
		if (*iter == job)
		{
			m_executingJobs.erase(iter);
			break;
		}
	}
	m_executingJobsMutex.unlock();
}

Job* JobSystem::RetrieveFinishedJob()
{
	m_finishedJobsMutex.lock();
	Job* finishedJob = nullptr;
	if (!m_finishedJobs.empty())
	{
		finishedJob = m_finishedJobs.front();
		m_finishedJobs.pop_front();
	}
	m_finishedJobsMutex.unlock();
	return finishedJob;
}

int JobSystem::GetNumQueuedJobs() const
{
	m_queuedJobsMutex.lock();
	int num = (int)m_queuedJobs.size();
	m_queuedJobsMutex.unlock();
	return num;
}

int JobSystem::GetNumExecutingJobs() const
{
	m_executingJobsMutex.lock();
	int num = (int)m_executingJobs.size();
	m_executingJobsMutex.unlock();
	return num;
}

void JobSystem::CancelAllJobs()
{
	for (int i = 0; i < m_workerThreads.size(); i++)
	{
		m_workerThreads[i]->m_isQuitting = true;
	}
}

void JobSystem::DestroyAllThreads()
{
	for (int i = 0; i < m_workerThreads.size(); i++)
	{
		delete m_workerThreads[i];
		m_workerThreads[i] = nullptr;
	}
}

