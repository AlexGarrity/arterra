#pragma once

#include <cmath>

namespace arterra {

	namespace Maths {

		inline unsigned PMod(const int value, const unsigned m)
		{
			const auto r = value % static_cast<int>(m);
			return (r < 0) ? (r + m) : r;
		}
	}

}