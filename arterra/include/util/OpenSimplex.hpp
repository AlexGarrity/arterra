#pragma once

#include "PCH.hpp"

#include "util/Random.hpp"

namespace arterra {

	/*
		All credit to someone who I can only assume is the secondcoming of Christ...
		https://gist.github.com/KdotJPG/b1270127455a94ac5d19
		Even more credit (yep, more than all) for making it public domain
		<3

		I'll maybe try to vectorise this at some point, release that public domain too
	*/

	class OpenSimplex3D {
		public:
			OpenSimplex3D();
			float Generate(float x, float y, float z);

		private:
			float Extrapolate(int x, int y, int z, float dX, float dY, float dZ);


			const float _stretchConstant = -1.0f / 6.0f;
			const float _squishConstant = 1.0f / 3.0f;
			const float _normConstant = 103;

			std::vector<uint16_t> _permutations;
			std::vector<uint16_t> _permGradIndex;
			std::vector<int8_t> _gradients {
				-11,  4,  4,     -4,  11,  4,    -4,  4,  11,
				11,  4,  4,      4,  11,  4,     4,  4,  11,
				-11, -4,  4,     -4, -11,  4,    -4, -4,  11,
				11, -4,  4,      4, -11,  4,     4, -4,  11,
				-11,  4, -4,     -4,  11, -4,    -4,  4, -11,
				11,  4, -4,      4,  11, -4,     4,  4, -11,
				-11, -4, -4,     -4, -11, -4,    -4, -4, -11,
				11, -4, -4,      4, -11, -4,     4, -4, -11
			};

			Random _randomGenerator;
	};


	class OpenSimplex2D {
		public:
			OpenSimplex2D();
			float Generate(float x, float y);

		private:
			float Extrapolate(int x, int y, float dX, float dY);

			const float _stretchConstant = -0.211324865405187f;
			const float _squishConstant = 0.366025403784439f;
			const float _normConstant = 47.0f;

			std::vector<uint16_t> _permutations;
			std::vector<uint16_t> _permGradIndex;
			std::vector<int8_t> _gradients {
				-11,  4,  4,     -4,  11,  4,    -4,  4,  11,
				11,  4,  4,      4,  11,  4,     4,  4,  11,
				-11, -4,  4,     -4, -11,  4,    -4, -4,  11,
				11, -4,  4,      4, -11,  4,     4, -4,  11,
				-11,  4, -4,     -4,  11, -4,    -4,  4, -11,
				11,  4, -4,      4,  11, -4,     4,  4, -11,
				-11, -4, -4,     -4, -11, -4,    -4, -4, -11,
				11, -4, -4,      4, -11, -4,     4, -4, -11
			};

			Random _randomGenerator;
	};

}