#pragma once

#include "PCH.hpp"

namespace arterra {

	struct ThreadJob {
		ThreadJob();
		ThreadJob(std::function<void()>&& job, size_t priority = 0);
		ThreadJob(const ThreadJob& other);
		ThreadJob(const ThreadJob&& other);

		void operator=(const ThreadJob &other);
		void operator()() const;
		
		std::function<void()> _job;
		size_t _priority;
	};

	bool operator<(const ThreadJob& a, const ThreadJob& b);
	bool operator>(const ThreadJob& a, const ThreadJob& b);
}
