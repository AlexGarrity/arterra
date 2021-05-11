#pragma once

#include <cmath>

namespace arterra {

	namespace Maths {

		unsigned PMod(int value, unsigned m)
		{
			int r = value % m;
			return (r < 0) ? (r + m) : r;
		}
	}

}