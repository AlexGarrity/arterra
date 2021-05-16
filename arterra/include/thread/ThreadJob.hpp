#pragma once

#include "PCH.hpp"

namespace arterra {

	struct ThreadJob {
		ThreadJob(std::function<void()>&& job);
		const std::function<void()> _job;
	};

}
