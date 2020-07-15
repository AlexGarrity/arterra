#pragma once

#include <cmath>

namespace arterra {

	namespace Maths {

		int PMod(int value, int m)
		{
			if (value < 0) {
				int mod = (-value) % m;
				return (mod == 0) ? 0 : m - mod;
			}
			return value % m;
		}

	}

}