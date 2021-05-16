#pragma once

#include "PCH.hpp"

#include "thread/ThreadJob.hpp"

namespace arterra {

	class ThreadManager {
	public:
		ThreadManager();
		~ThreadManager();

		void PushJob(ThreadJob& threadJob);

		void CreateThreads(size_t threadCount = 3);
		void TerminateThreads();
		
	private:
		void ThreadFunction();

		std::priority_queue<ThreadJob> _jobQueue;
		std::vector<std::thread*> _threadPool;
		const std::size_t _systemThreadCount;

		bool _awaitingShutdown;
		std::mutex _threadJobLock;
		std::condition_variable _threadCondition;
	};


}
