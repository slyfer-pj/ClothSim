#pragma once
#include <thread>
#include <atomic>

class JobSystem;

class JobWorkerThread
{
	friend class JobSystem;
public:
	JobWorkerThread(JobSystem* jobSystem, int workerThreadID);
	~JobWorkerThread();
	void JobWorkerMain(int workerID);

private:
	JobSystem* m_jobSystem;
	int m_workerThreadID = -1;
	std::thread* m_thread = nullptr;
	std::atomic<bool> m_isQuitting = false;
};