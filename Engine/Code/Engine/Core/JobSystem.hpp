#pragma once
#include <deque>
#include <mutex>
#include <vector>

class Job;
class JobWorkerThread;

struct JobSystemConfig
{
	int m_numWorkerThreads = 8;
};

class JobSystem
{
public:
	JobSystem(const JobSystemConfig& config);
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();

	void QueueJobs(Job* jobToExecute);
	Job* ClaimJobToExecute();
	void MoveJobToFinishedQueue(Job* job);
	Job* RetrieveFinishedJob();
	int GetNumQueuedJobs() const;
	int GetNumExecutingJobs() const;

	void CancelAllJobs();

private:
	JobSystemConfig m_config;
	std::deque<Job*> m_queuedJobs;
	mutable std::mutex m_queuedJobsMutex;

	std::vector<Job*> m_executingJobs;
	mutable std::mutex m_executingJobsMutex;

	std::deque<Job*> m_finishedJobs;
	mutable std::mutex m_finishedJobsMutex;

	std::vector<JobWorkerThread*> m_workerThreads;

private:
	void DestroyAllThreads();
};