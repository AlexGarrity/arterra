#include "thread/ThreadJob.hpp"

namespace arterra {

	ThreadJob::ThreadJob()
		: _priority{ 0 } { }

	ThreadJob::ThreadJob(std::function<void()>&& job, const size_t priority)
		: _job{ std::move(job) }
		  , _priority{ priority } { }

	ThreadJob::ThreadJob(const ThreadJob& other)
		: _job{ std::move(other._job) }
		  , _priority{ other._priority } { }

	ThreadJob::ThreadJob(const ThreadJob&& other)
		: _job{ std::move(other._job) }
		  , _priority{ other._priority } { }

	void ThreadJob::operator=(const ThreadJob& other)
	{
		_job = other._job;
		_priority = other._priority;
	}

	void ThreadJob::operator()() const { _job(); }

	bool operator<(const ThreadJob& a, const ThreadJob& b) { return a._priority < b._priority; }

	bool operator>(const ThreadJob& a, const ThreadJob& b) { return a._priority > b._priority; }

}
