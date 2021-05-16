#include "thread/ThreadManager.hpp"

namespace arterra {

	ThreadManager::ThreadManager()
		: _awaitingShutdown{ false }
		  , _systemThreadCount(std::thread::hardware_concurrency()) { }

	ThreadManager::~ThreadManager()
	{
		TerminateThreads();
	}

	void ThreadManager::PushJob(ThreadJob& threadJob)
	{
		{
			std::unique_lock<std::mutex> lock(_threadJobLock);
			_jobQueue.push(std::move(threadJob));
		}
		_threadCondition.notify_one();
	}

	void ThreadManager::CreateThreads(const size_t threadCount)
	{
		for (std::size_t i = 0; i < std::min(threadCount, _systemThreadCount - 1); ++i) {
			auto* t = new std::thread([this] { ThreadFunction(); });
			_threadPool.emplace_back(t);
		}
	}

	void ThreadManager::TerminateThreads()
	{
		std::unique_lock<std::mutex> lock(_threadJobLock);
		_awaitingShutdown = true;
		_threadCondition.notify_all();

		for (const auto& t : _threadPool) {
			t->join();
			delete t;
		}
	}

	void ThreadManager::ThreadFunction()
	{
		std::function<void()> j;
		while (true) {
			{
				std::unique_lock<std::mutex> lock(_threadJobLock);
				_threadCondition.wait(lock, [this] {
                    return _awaitingShutdown | !_jobQueue.empty(); });
				j = _jobQueue.front()._job;
				_jobQueue.pop();
			}
			j();
		}
	}


}
