#pragma once

namespace arterra {

	namespace Maths {

		int PMod(int value, int m)
		{
			if (value < 0)
				return m - ((-value) % m);
			return value % m;
		}

	}

}