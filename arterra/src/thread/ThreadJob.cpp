#include "thread/ThreadJob.hpp"

namespace arterra {

	ThreadJob::ThreadJob(std::function<void()> &&job)
		: _job{std::move(job)}
	{
		
	}

}
