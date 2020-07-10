#include "util/OpenSimplex.hpp"

namespace arterra {

	OpenSimplex3D::OpenSimplex3D()
	{
		size_t seed = _randomGenerator.Generate();
		_permutations.resize(256);
		_permGradIndex.resize(256);
		std::array<uint16_t, 256> source;
		for (auto i = 0; i < 256; i++)
			source[i] = i;
		seed = seed * 6364136223846793005l + 1442695040888963407l;
		seed = seed * 6364136223846793005l + 1442695040888963407l;
		seed = seed * 6364136223846793005l + 1442695040888963407l;
		for (int i = 255; i >= 0; i--) {
			seed = seed * 6364136223846793005l + 1442695040888963407l;
			int r = (int)((seed + 31) % (i + 1));
			if (r < 0)
				r += (i + 1);
			_permutations[i] = source[r];
			_permGradIndex[i] = static_cast<uint16_t>((_permutations[i] % (_gradients.size() / 3)) * 3);
			source[r] = source[i];
		}
	}

	float OpenSimplex3D::Generate(float x, float y, float z)
	{
		// Place coordinates onto honeycomb
		float stretchOffset = (x + y + z) * _stretchConstant;
		float xs = x + stretchOffset;
		float ys = y + stretchOffset;
		float zs = z + stretchOffset;

		// Floor the honeycomb coordinates to get the super-cell origin?
		int xsb = xs;
		int ysb = ys;
		int zsb = zs;

		// Get the actual coordinates of the origin
		float squishOffset = (xsb + ysb + zsb) * _squishConstant;
		float xb = xsb + squishOffset;
		float yb = ysb + squishOffset;
		float zb = zsb + squishOffset;

		// Calculate the coordinates relative to the origin
		float xins = xs - xsb;
		float yins = ys - ysb;
		float zins = zs - zsb;

		// Sum the coordinates to get which region we're in
		float inSum = xins + yins + zins;

		// Get the position relative to the origin
		float dx0 = x - xb;
		float dy0 = y - yb;
		float dz0 = z - zb;

		float dx_ext0, dy_ext0, dz_ext0;
		float dx_ext1, dy_ext1, dz_ext1;
		int xsv_ext0, ysv_ext0, zsv_ext0;
		int xsv_ext1, ysv_ext1, zsv_ext1;

		float value = 0;
		if (inSum <= 1.0f) {

			// Determine which two of (0,0,1), (0,1,0), (1,0,0) are closest.
			int8_t aPoint = 0x01;
			float aScore = xins;
			int8_t bPoint = 0x02;
			float bScore = yins;
			if (aScore >= bScore && zins > aScore) {
				bScore = zins;
				bPoint = 0x04;
			} else if (aScore < bScore && zins > aScore) {
				aScore = zins;
				aPoint = 0x04;
			}

			// Determine the two lattice points not part of the tetrahedron
			float wins = 1 - inSum;
			if (wins > aScore || wins > bScore) {
				int8_t c = (bScore > aScore) ? bPoint : aPoint;

				if (!(c & 0x01)) {
					xsv_ext0 = xsb - 1;
					xsv_ext1 = xsb;
					dx_ext0 = dx0 + 1.0f;
					dx_ext1 = dx0;
				} else {
					xsv_ext0 = xsv_ext1 = xsb + 1;
					dx_ext0 = dx_ext1 = dx0 - 1.0f;
				}

				if ((c & 0x02) == 0) {
					ysv_ext0 = ysv_ext1 = ysb;
					dy_ext0 = dy_ext1 = dy0;
					if ((c & 0x01) == 0) {
						ysv_ext1 -= 1;
						dy_ext1 += 1.0f;
					} else {
						ysv_ext0 -= 1;
						dy_ext0 += 1.0f;
					}
				} else {
					ysv_ext0 = ysv_ext1 = ysb + 1;
					dy_ext0 = dy_ext1 = dy0 - 1.0f;
				}

				if ((c & 0x04) == 0) {
					zsv_ext0 = zsb;
					zsv_ext1 = zsb - 1;
					dz_ext0 = dz0;
					dz_ext1 = dz0 + 1;
				} else {
					zsv_ext0 = zsv_ext1 = zsb + 1;
					dz_ext0 = dz_ext1 = dz0 - 1;
				}

			} else { //(0,0,0) is not one of the closest two tetrahedral vertices.
				int8_t c = static_cast<int8_t>(
					aPoint | bPoint); // Our two extra vertices are determined by the closest two.

				if ((c & 0x01) == 0) {
					xsv_ext0 = xsb;
					xsv_ext1 = xsb - 1;
					dx_ext0 = dx0 - 2.0f * _squishConstant;
					dx_ext1 = dx0 + 1.0f - _squishConstant;
				} else {
					xsv_ext0 = xsv_ext1 = xsb + 1;
					dx_ext0 = dx0 - 1.0f - 2.0f * _squishConstant;
					dx_ext1 = dx0 - 1.0f - _squishConstant;
				}

				if ((c & 0x02) == 0) {
					ysv_ext0 = ysb;
					ysv_ext1 = ysb - 1;
					dy_ext0 = dy0 - 2.0f * _squishConstant;
					dy_ext1 = dy0 + 1.0f - _squishConstant;
				} else {
					ysv_ext0 = ysv_ext1 = ysb + 1;
					dy_ext0 = dy0 - 1.0f - 2.0f * _squishConstant;
					dy_ext1 = dy0 - 1.0f - _squishConstant;
				}

				if ((c & 0x04) == 0) {
					zsv_ext0 = zsb;
					zsv_ext1 = zsb - 1;
					dz_ext0 = dz0 - 2.0f * _squishConstant;
					dz_ext1 = dz0 + 1.0f - _squishConstant;
				} else {
					zsv_ext0 = zsv_ext1 = zsb + 1;
					dz_ext0 = dz0 - 1.0f - 2.0f * _squishConstant;
					dz_ext1 = dz0 - 1.0f - _squishConstant;
				}
			}

			// Contribution (0,0,0)
			float attn0 = 2 - dx0 * dx0 - dy0 * dy0 - dz0 * dz0;
			if (attn0 > 0) {
				attn0 *= attn0;
				value += attn0 * attn0 * Extrapolate(xsb + 0, ysb + 0, zsb + 0, dx0, dy0, dz0);
			}

			// Contribution (1,0,0)
			float dx1 = dx0 - 1.0f - _squishConstant;
			float dy1 = dy0 - 0.0f - _squishConstant;
			float dz1 = dz0 - 0.0f - _squishConstant;
			float attn1 = 2.0f - dx1 * dx1 - dy1 * dy1 - dz1 * dz1;
			if (attn1 > 0.0f) {
				attn1 *= attn1;
				value += attn1 * attn1 * Extrapolate(xsb + 1, ysb + 0, zsb + 0, dx1, dy1, dz1);
			}

			// Contribution (0,1,0)
			float dx2 = dx0 - 0.0f - _squishConstant;
			float dy2 = dy0 - 1.0f - _squishConstant;
			float dz2 = dz1;
			float attn2 = 2.0f - dx2 * dx2 - dy2 * dy2 - dz2 * dz2;
			if (attn2 > 0.0f) {
				attn2 *= attn2;
				value += attn2 * attn2 * Extrapolate(xsb + 0, ysb + 1, zsb + 0, dx2, dy2, dz2);
			}

			// Contribution (0,0,1)
			float dx3 = dx2;
			float dy3 = dy1;
			float dz3 = dz0 - 1.0f - _squishConstant;
			float attn3 = 2.0f - dx3 * dx3 - dy3 * dy3 - dz3 * dz3;
			if (attn3 > 0.0f) {
				attn3 *= attn3;
				value += attn3 * attn3 * Extrapolate(xsb + 0, ysb + 0, zsb + 1, dx3, dy3, dz3);
			}

		} else if (inSum >= 2) { // We're inside the tetrahedron (3-Simplex) at (1,1,1)

			// Determine which two tetrahedral vertices are the closest, out of (1,1,0), (1,0,1), (0,1,1) but not
			// (1,1,1).
			int8_t aPoint = 0x06;
			float aScore = xins;
			int8_t bPoint = 0x05;
			float bScore = yins;
			if (aScore <= bScore && zins < bScore) {
				bScore = zins;
				bPoint = 0x03;
			} else if (aScore > bScore && zins < aScore) {
				aScore = zins;
				aPoint = 0x03;
			}

			// Now we determine the two lattice points not part of the tetrahedron that may contribute.
			// This depends on the closest two tetrahedral vertices, including (1,1,1)
			float wins = 3 - inSum;
			if (wins < aScore || wins < bScore) { //(1,1,1) is one of the closest two tetrahedral vertices.
				int8_t c = (bScore < aScore ? bPoint
											: aPoint); // Our other closest vertex is the closest out of a and b.

				if ((c & 0x01) != 0) {
					xsv_ext0 = xsb + 2;
					xsv_ext1 = xsb + 1;
					dx_ext0 = dx0 - 2 - 3 * _squishConstant;
					dx_ext1 = dx0 - 1 - 3 * _squishConstant;
				} else {
					xsv_ext0 = xsv_ext1 = xsb;
					dx_ext0 = dx_ext1 = dx0 - 3 * _squishConstant;
				}

				if ((c & 0x02) != 0) {
					ysv_ext0 = ysv_ext1 = ysb + 1;
					dy_ext0 = dy_ext1 = dy0 - 1 - 3 * _squishConstant;
					if ((c & 0x01) != 0) {
						ysv_ext1 += 1;
						dy_ext1 -= 1;
					} else {
						ysv_ext0 += 1;
						dy_ext0 -= 1;
					}
				} else {
					ysv_ext0 = ysv_ext1 = ysb;
					dy_ext0 = dy_ext1 = dy0 - 3 * _squishConstant;
				}

				if ((c & 0x04) != 0) {
					zsv_ext0 = zsb + 1;
					zsv_ext1 = zsb + 2;
					dz_ext0 = dz0 - 1 - 3 * _squishConstant;
					dz_ext1 = dz0 - 2 - 3 * _squishConstant;
				} else {
					zsv_ext0 = zsv_ext1 = zsb;
					dz_ext0 = dz_ext1 = dz0 - 3 * _squishConstant;
				}
			} else { //(1,1,1) is not one of the closest two tetrahedral vertices.
				int8_t c = (int8_t)(aPoint & bPoint); // Our two extra vertices are determined by the closest two.

				if ((c & 0x01) != 0) {
					xsv_ext0 = xsb + 1;
					xsv_ext1 = xsb + 2;
					dx_ext0 = dx0 - 1 - _squishConstant;
					dx_ext1 = dx0 - 2 - 2 * _squishConstant;
				} else {
					xsv_ext0 = xsv_ext1 = xsb;
					dx_ext0 = dx0 - _squishConstant;
					dx_ext1 = dx0 - 2 * _squishConstant;
				}

				if ((c & 0x02) != 0) {
					ysv_ext0 = ysb + 1;
					ysv_ext1 = ysb + 2;
					dy_ext0 = dy0 - 1 - _squishConstant;
					dy_ext1 = dy0 - 2 - 2 * _squishConstant;
				} else {
					ysv_ext0 = ysv_ext1 = ysb;
					dy_ext0 = dy0 - _squishConstant;
					dy_ext1 = dy0 - 2 * _squishConstant;
				}

				if ((c & 0x04) != 0) {
					zsv_ext0 = zsb + 1;
					zsv_ext1 = zsb + 2;
					dz_ext0 = dz0 - 1 - _squishConstant;
					dz_ext1 = dz0 - 2 - 2 * _squishConstant;
				} else {
					zsv_ext0 = zsv_ext1 = zsb;
					dz_ext0 = dz0 - _squishConstant;
					dz_ext1 = dz0 - 2 * _squishConstant;
				}
			}

			// Contribution (1,1,0)
			float dx3 = dx0 - 1 - 2 * _squishConstant;
			float dy3 = dy0 - 1 - 2 * _squishConstant;
			float dz3 = dz0 - 0 - 2 * _squishConstant;
			float attn3 = 2 - dx3 * dx3 - dy3 * dy3 - dz3 * dz3;
			if (attn3 > 0) {
				attn3 *= attn3;
				value += attn3 * attn3 * Extrapolate(xsb + 1, ysb + 1, zsb + 0, dx3, dy3, dz3);
			}

			// Contribution (1,0,1)
			float dx2 = dx3;
			float dy2 = dy0 - 0 - 2 * _squishConstant;
			float dz2 = dz0 - 1 - 2 * _squishConstant;
			float attn2 = 2 - dx2 * dx2 - dy2 * dy2 - dz2 * dz2;
			if (attn2 > 0) {
				attn2 *= attn2;
				value += attn2 * attn2 * Extrapolate(xsb + 1, ysb + 0, zsb + 1, dx2, dy2, dz2);
			}

			// Contribution (0,1,1)
			float dx1 = dx0 - 0 - 2 * _squishConstant;
			float dy1 = dy3;
			float dz1 = dz2;
			float attn1 = 2 - dx1 * dx1 - dy1 * dy1 - dz1 * dz1;
			if (attn1 > 0) {
				attn1 *= attn1;
				value += attn1 * attn1 * Extrapolate(xsb + 0, ysb + 1, zsb + 1, dx1, dy1, dz1);
			}

			// Contribution (1,1,1)
			dx0 = dx0 - 1 - 3 * _squishConstant;
			dy0 = dy0 - 1 - 3 * _squishConstant;
			dz0 = dz0 - 1 - 3 * _squishConstant;
			float attn0 = 2 - dx0 * dx0 - dy0 * dy0 - dz0 * dz0;
			if (attn0 > 0) {
				attn0 *= attn0;
				value += attn0 * attn0 * Extrapolate(xsb + 1, ysb + 1, zsb + 1, dx0, dy0, dz0);
			}
		} else { // We're inside the octahedron (Rectified 3-Simplex) in between.
			float aScore;
			int8_t aPoint;
			bool aIsFurtherSide;
			float bScore;
			int8_t bPoint;
			bool bIsFurtherSide;

			// Decide between point (0,0,1) and (1,1,0) as closest
			float p1 = xins + yins;
			if (p1 > 1) {
				aScore = p1 - 1;
				aPoint = 0x03;
				aIsFurtherSide = true;
			} else {
				aScore = 1 - p1;
				aPoint = 0x04;
				aIsFurtherSide = false;
			}

			// Decide between point (0,1,0) and (1,0,1) as closest
			float p2 = xins + zins;
			if (p2 > 1) {
				bScore = p2 - 1;
				bPoint = 0x05;
				bIsFurtherSide = true;
			} else {
				bScore = 1 - p2;
				bPoint = 0x02;
				bIsFurtherSide = false;
			}

			// The closest out of the two (1,0,0) and (0,1,1) will replace the furthest out of the two decided
			// above, if closer.
			float p3 = yins + zins;
			if (p3 > 1) {
				float score = p3 - 1;
				if (aScore <= bScore && aScore < score) {
					aScore = score;
					aPoint = 0x06;
					aIsFurtherSide = true;
				} else if (aScore > bScore && bScore < score) {
					bScore = score;
					bPoint = 0x06;
					bIsFurtherSide = true;
				}
			} else {
				float score = 1 - p3;
				if (aScore <= bScore && aScore < score) {
					aScore = score;
					aPoint = 0x01;
					aIsFurtherSide = false;
				} else if (aScore > bScore && bScore < score) {
					bScore = score;
					bPoint = 0x01;
					bIsFurtherSide = false;
				}
			}

			// Where each of the two closest points are determines how the extra two vertices are calculated.
			if (aIsFurtherSide == bIsFurtherSide) {
				if (aIsFurtherSide) { // Both closest points on (1,1,1) side

					// One of the two extra points is (1,1,1)
					dx_ext0 = dx0 - 1 - 3 * _squishConstant;
					dy_ext0 = dy0 - 1 - 3 * _squishConstant;
					dz_ext0 = dz0 - 1 - 3 * _squishConstant;
					xsv_ext0 = xsb + 1;
					ysv_ext0 = ysb + 1;
					zsv_ext0 = zsb + 1;

					// Other extra point is based on the shared axis.
					int8_t c = (int8_t)(aPoint & bPoint);
					if ((c & 0x01) != 0) {
						dx_ext1 = dx0 - 2 - 2 * _squishConstant;
						dy_ext1 = dy0 - 2 * _squishConstant;
						dz_ext1 = dz0 - 2 * _squishConstant;
						xsv_ext1 = xsb + 2;
						ysv_ext1 = ysb;
						zsv_ext1 = zsb;
					} else if ((c & 0x02) != 0) {
						dx_ext1 = dx0 - 2 * _squishConstant;
						dy_ext1 = dy0 - 2 - 2 * _squishConstant;
						dz_ext1 = dz0 - 2 * _squishConstant;
						xsv_ext1 = xsb;
						ysv_ext1 = ysb + 2;
						zsv_ext1 = zsb;
					} else {
						dx_ext1 = dx0 - 2 * _squishConstant;
						dy_ext1 = dy0 - 2 * _squishConstant;
						dz_ext1 = dz0 - 2 - 2 * _squishConstant;
						xsv_ext1 = xsb;
						ysv_ext1 = ysb;
						zsv_ext1 = zsb + 2;
					}
				} else { // Both closest points on (0,0,0) side

					// One of the two extra points is (0,0,0)
					dx_ext0 = dx0;
					dy_ext0 = dy0;
					dz_ext0 = dz0;
					xsv_ext0 = xsb;
					ysv_ext0 = ysb;
					zsv_ext0 = zsb;

					// Other extra point is based on the omitted axis.
					int8_t c = (int8_t)(aPoint | bPoint);
					if ((c & 0x01) == 0) {
						dx_ext1 = dx0 + 1 - _squishConstant;
						dy_ext1 = dy0 - 1 - _squishConstant;
						dz_ext1 = dz0 - 1 - _squishConstant;
						xsv_ext1 = xsb - 1;
						ysv_ext1 = ysb + 1;
						zsv_ext1 = zsb + 1;
					} else if ((c & 0x02) == 0) {
						dx_ext1 = dx0 - 1 - _squishConstant;
						dy_ext1 = dy0 + 1 - _squishConstant;
						dz_ext1 = dz0 - 1 - _squishConstant;
						xsv_ext1 = xsb + 1;
						ysv_ext1 = ysb - 1;
						zsv_ext1 = zsb + 1;
					} else {
						dx_ext1 = dx0 - 1 - _squishConstant;
						dy_ext1 = dy0 - 1 - _squishConstant;
						dz_ext1 = dz0 + 1 - _squishConstant;
						xsv_ext1 = xsb + 1;
						ysv_ext1 = ysb + 1;
						zsv_ext1 = zsb - 1;
					}
				}
			} else { // One point on (0,0,0) side, one point on (1,1,1) side
				int8_t c1, c2;
				if (aIsFurtherSide) {
					c1 = aPoint;
					c2 = bPoint;
				} else {
					c1 = bPoint;
					c2 = aPoint;
				}

				// One contribution is a permutation of (1,1,-1)
				if ((c1 & 0x01) == 0) {
					dx_ext0 = dx0 + 1 - _squishConstant;
					dy_ext0 = dy0 - 1 - _squishConstant;
					dz_ext0 = dz0 - 1 - _squishConstant;
					xsv_ext0 = xsb - 1;
					ysv_ext0 = ysb + 1;
					zsv_ext0 = zsb + 1;
				} else if ((c1 & 0x02) == 0) {
					dx_ext0 = dx0 - 1 - _squishConstant;
					dy_ext0 = dy0 + 1 - _squishConstant;
					dz_ext0 = dz0 - 1 - _squishConstant;
					xsv_ext0 = xsb + 1;
					ysv_ext0 = ysb - 1;
					zsv_ext0 = zsb + 1;
				} else {
					dx_ext0 = dx0 - 1 - _squishConstant;
					dy_ext0 = dy0 - 1 - _squishConstant;
					dz_ext0 = dz0 + 1 - _squishConstant;
					xsv_ext0 = xsb + 1;
					ysv_ext0 = ysb + 1;
					zsv_ext0 = zsb - 1;
				}

				// One contribution is a permutation of (0,0,2)
				dx_ext1 = dx0 - 2 * _squishConstant;
				dy_ext1 = dy0 - 2 * _squishConstant;
				dz_ext1 = dz0 - 2 * _squishConstant;
				xsv_ext1 = xsb;
				ysv_ext1 = ysb;
				zsv_ext1 = zsb;
				if ((c2 & 0x01) != 0) {
					dx_ext1 -= 2;
					xsv_ext1 += 2;
				} else if ((c2 & 0x02) != 0) {
					dy_ext1 -= 2;
					ysv_ext1 += 2;
				} else {
					dz_ext1 -= 2;
					zsv_ext1 += 2;
				}
			}

			// Contribution (1,0,0)
			float dx1 = dx0 - 1 - _squishConstant;
			float dy1 = dy0 - 0 - _squishConstant;
			float dz1 = dz0 - 0 - _squishConstant;
			float attn1 = 2 - dx1 * dx1 - dy1 * dy1 - dz1 * dz1;
			if (attn1 > 0) {
				attn1 *= attn1;
				value += attn1 * attn1 * Extrapolate(xsb + 1, ysb + 0, zsb + 0, dx1, dy1, dz1);
			}

			// Contribution (0,1,0)
			float dx2 = dx0 - 0 - _squishConstant;
			float dy2 = dy0 - 1 - _squishConstant;
			float dz2 = dz1;
			float attn2 = 2 - dx2 * dx2 - dy2 * dy2 - dz2 * dz2;
			if (attn2 > 0) {
				attn2 *= attn2;
				value += attn2 * attn2 * Extrapolate(xsb + 0, ysb + 1, zsb + 0, dx2, dy2, dz2);
			}

			// Contribution (0,0,1)
			float dx3 = dx2;
			float dy3 = dy1;
			float dz3 = dz0 - 1 - _squishConstant;
			float attn3 = 2 - dx3 * dx3 - dy3 * dy3 - dz3 * dz3;
			if (attn3 > 0) {
				attn3 *= attn3;
				value += attn3 * attn3 * Extrapolate(xsb + 0, ysb + 0, zsb + 1, dx3, dy3, dz3);
			}

			// Contribution (1,1,0)
			float dx4 = dx0 - 1 - 2 * _squishConstant;
			float dy4 = dy0 - 1 - 2 * _squishConstant;
			float dz4 = dz0 - 0 - 2 * _squishConstant;
			float attn4 = 2 - dx4 * dx4 - dy4 * dy4 - dz4 * dz4;
			if (attn4 > 0) {
				attn4 *= attn4;
				value += attn4 * attn4 * Extrapolate(xsb + 1, ysb + 1, zsb + 0, dx4, dy4, dz4);
			}

			// Contribution (1,0,1)
			float dx5 = dx4;
			float dy5 = dy0 - 0 - 2 * _squishConstant;
			float dz5 = dz0 - 1 - 2 * _squishConstant;
			float attn5 = 2 - dx5 * dx5 - dy5 * dy5 - dz5 * dz5;
			if (attn5 > 0) {
				attn5 *= attn5;
				value += attn5 * attn5 * Extrapolate(xsb + 1, ysb + 0, zsb + 1, dx5, dy5, dz5);
			}

			// Contribution (0,1,1)
			float dx6 = dx0 - 0 - 2 * _squishConstant;
			float dy6 = dy4;
			float dz6 = dz5;
			float attn6 = 2 - dx6 * dx6 - dy6 * dy6 - dz6 * dz6;
			if (attn6 > 0) {
				attn6 *= attn6;
				value += attn6 * attn6 * Extrapolate(xsb + 0, ysb + 1, zsb + 1, dx6, dy6, dz6);
			}
		}

		// First extra vertex
		float attn_ext0 = 2 - dx_ext0 * dx_ext0 - dy_ext0 * dy_ext0 - dz_ext0 * dz_ext0;
		if (attn_ext0 > 0) {
			attn_ext0 *= attn_ext0;
			value += attn_ext0 * attn_ext0 * Extrapolate(xsv_ext0, ysv_ext0, zsv_ext0, dx_ext0, dy_ext0, dz_ext0);
		}

		// Second extra vertex
		float attn_ext1 = 2 - dx_ext1 * dx_ext1 - dy_ext1 * dy_ext1 - dz_ext1 * dz_ext1;
		if (attn_ext1 > 0) {
			attn_ext1 *= attn_ext1;
			value += attn_ext1 * attn_ext1 * Extrapolate(xsv_ext1, ysv_ext1, zsv_ext1, dx_ext1, dy_ext1, dz_ext1);
		}

		return value / _normConstant;
	}

	float OpenSimplex3D::Extrapolate(int xsb, int ysb, int zsb, float dx, float dy, float dz)
	{
		int index = _permGradIndex[(_permutations[(_permutations[xsb & 0xFF] + ysb) & 0xFF] + zsb) & 0xFF];
		return _gradients[index] * dx + _gradients[index + 1] * dy + _gradients[index + 2] * dz;
	}


	OpenSimplex2D::OpenSimplex2D()
	{
		size_t seed = _randomGenerator.Generate();
		_permutations.resize(256);
		_permGradIndex.resize(256);
		std::array<uint16_t, 256> source;
		for (auto i = 0; i < 256; i++)
			source[i] = i;
		seed = seed * 6364136223846793005l + 1442695040888963407l;
		seed = seed * 6364136223846793005l + 1442695040888963407l;
		seed = seed * 6364136223846793005l + 1442695040888963407l;
		for (int i = 255; i >= 0; i--) {
			seed = seed * 6364136223846793005l + 1442695040888963407l;
			int r = (int)((seed + 31) % (i + 1));
			if (r < 0)
				r += (i + 1);
			_permutations[i] = source[r];
			_permGradIndex[i] = static_cast<uint16_t>((_permutations[i] % (_gradients.size() / 3)) * 3);
			source[r] = source[i];
		}
	}

	float OpenSimplex2D::Generate(float x, float y) {
	
		//Place input coordinates onto grid.
		float stretchOffset = (x + y) * _stretchConstant;
		float xs = x + stretchOffset;
		float ys = y + stretchOffset;
		
		//Floor to get grid coordinates of rhombus (stretched square) super-cell origin.
		int xsb = static_cast<int>(xs);
		int ysb = static_cast<int>(ys);
		
		//Skew out to get actual coordinates of rhombus origin. We'll need these later.
		float squishOffset = (xsb + ysb) * _squishConstant;
		float xb = xsb + squishOffset;
		float yb = ysb + squishOffset;
		
		//Compute grid coordinates relative to rhombus origin.
		float xins = xs - xsb;
		float yins = ys - ysb;
		
		//Sum those together to get a value that determines which region we're in.
		float inSum = xins + yins;

		//Positions relative to origin point.
		float dx0 = x - xb;
		float dy0 = y - yb;
		
		//We'll be defining these inside the next block and using them afterwards.
		float dx_ext, dy_ext;
		int xsv_ext, ysv_ext;
		
		float value = 0;

		//Contribution (1,0)
		float dx1 = dx0 - 1 - _squishConstant;
		float dy1 = dy0 - 0 - _squishConstant;
		float attn1 = 2 - dx1 * dx1 - dy1 * dy1;
		if (attn1 > 0) {
			attn1 *= attn1;
			value += attn1 * attn1 * Extrapolate(xsb + 1, ysb + 0, dx1, dy1);
		}

		//Contribution (0,1)
		float dx2 = dx0 - 0 - _squishConstant;
		float dy2 = dy0 - 1 - _squishConstant;
		float attn2 = 2 - dx2 * dx2 - dy2 * dy2;
		if (attn2 > 0) {
			attn2 *= attn2;
			value += attn2 * attn2 * Extrapolate(xsb + 0, ysb + 1, dx2, dy2);
		}
		
		if (inSum <= 1) { //We're inside the triangle (2-Simplex) at (0,0)
			float zins = 1 - inSum;
			if (zins > xins || zins > yins) { //(0,0) is one of the closest two triangular vertices
				if (xins > yins) {
					xsv_ext = xsb + 1;
					ysv_ext = ysb - 1;
					dx_ext = dx0 - 1;
					dy_ext = dy0 + 1;
				} else {
					xsv_ext = xsb - 1;
					ysv_ext = ysb + 1;
					dx_ext = dx0 + 1;
					dy_ext = dy0 - 1;
				}
			} else { //(1,0) and (0,1) are the closest two vertices.
				xsv_ext = xsb + 1;
				ysv_ext = ysb + 1;
				dx_ext = dx0 - 1 - 2 * _squishConstant;
				dy_ext = dy0 - 1 - 2 * _squishConstant;
			}
		} else { //We're inside the triangle (2-Simplex) at (1,1)
			float zins = 2 - inSum;
			if (zins < xins || zins < yins) { //(0,0) is one of the closest two triangular vertices
				if (xins > yins) {
					xsv_ext = xsb + 2;
					ysv_ext = ysb + 0;
					dx_ext = dx0 - 2 - 2 * _squishConstant;
					dy_ext = dy0 + 0 - 2 * _squishConstant;
				} else {
					xsv_ext = xsb + 0;
					ysv_ext = ysb + 2;
					dx_ext = dx0 + 0 - 2 * _squishConstant;
					dy_ext = dy0 - 2 - 2 * _squishConstant;
				}
			} else { //(1,0) and (0,1) are the closest two vertices.
				dx_ext = dx0;
				dy_ext = dy0;
				xsv_ext = xsb;
				ysv_ext = ysb;
			}
			xsb += 1;
			ysb += 1;
			dx0 = dx0 - 1 - 2 * _squishConstant;
			dy0 = dy0 - 1 - 2 * _squishConstant;
		}
		
		//Contribution (0,0) or (1,1)
		float attn0 = 2 - dx0 * dx0 - dy0 * dy0;
		if (attn0 > 0) {
			attn0 *= attn0;
			value += attn0 * attn0 * Extrapolate(xsb, ysb, dx0, dy0);
		}
		
		//Extra Vertex
		float attn_ext = 2 - dx_ext * dx_ext - dy_ext * dy_ext;
		if (attn_ext > 0) {
			attn_ext *= attn_ext;
			value += attn_ext * attn_ext * Extrapolate(xsv_ext, ysv_ext, dx_ext, dy_ext);
		}
		
		return value / _normConstant;
	}

	float OpenSimplex2D::Extrapolate(int xsb, int ysb, float dx, float dy)
	{
		int index = _permutations[(_permutations[xsb & 0xFF] + ysb) & 0xFF] & 0x0E;
		return _gradients[index] * dx
			+ _gradients[index + 1] * dy;
	}

}