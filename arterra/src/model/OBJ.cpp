#include "model/OBJ.hpp"

namespace arterra {

	bool OBJ::Load(const std::vector<uint8_t> &data, fVec& vertOut, fVec& uvOut, fVec& normOut)
	{
		// Check that there's at least enough data for the header
		if (data.size() < 16) {
			// Data is too small
			return false;
		}

		// Check we have the right data
		auto magicNum = std::string(data.begin(), data.begin() + 4);
		if (magicNum != "MOBJ") {
			// Wrong data, I guess
			return false;
		}

		// Declare a bunch of variables
		uint16_t vertexLength = 0;
		uint16_t textureLength = 0;
		uint16_t normalCount = 0;
		uint16_t iV, iT, iN;

		// Declare a load of vectors
		std::vector<uint16_t> ivV, ivT, ivN;
		std::vector<float> vertices, uvs, normals;

		// Extract relevant data from the header
		vertexLength = static_cast<uint16_t>(data[4]) | static_cast<uint16_t>(data[5]) << 8;
		textureLength = static_cast<uint16_t>(data[6]) | static_cast<uint16_t>(data[7]) << 8;
		normalCount = static_cast<uint16_t>(data[8]) | static_cast<uint16_t>(data[9]) << 8;

		// Get length of indices (always equivalent; legacy code)
		iV = static_cast<uint16_t>(data[10]) | static_cast<uint16_t>(data[11]) << 8;
		iT = static_cast<uint16_t>(data[12]) | static_cast<uint16_t>(data[13]) << 8;
		iN = static_cast<uint16_t>(data[14]) | static_cast<uint16_t>(data[15]) << 8;

		// Reserve to reduce malloc calls
		vertices.reserve(vertexLength);
		uvs.reserve(textureLength);
		normals.reserve(normalCount);

		ivV.reserve(iV);
		ivT.reserve(iT);
		ivN.reserve(iN);

		// Set the offset to the current read position
		// = Where the header ends (after iN)
		size_t of = 16;
		for (auto x = 0; x < vertexLength * 4; x += 4) {
			uint32_t val = data[of] | data[of + 1] << 8 | data[of + 2] << 16 | data[of + 3] << 24;
			float fVal = *reinterpret_cast<float*>(&val);
			vertices.emplace_back(fVal);
			// Increase offset 4 bytes to account for read
			of += 4;
		}

		for (auto x = 0; x < textureLength * 4; x += 4) {
			uint32_t val = data[of] | data[of + 1] << 8 | data[of + 2] << 16 | data[of + 3] << 24;
			float fVal = *reinterpret_cast<float*>(&val);
			uvs.emplace_back(fVal);
			// Increase offset 4 bytes to account for read
			of += 4;
		}

		for (auto x = 0; x < normalCount * 4; x += 4) {
			uint32_t val = data[of] | data[of + 1] << 8 | data[of + 2] << 16 | data[of + 3] << 24;
			float fVal = *reinterpret_cast<float*>(&val);
			normals.emplace_back(fVal);
			// Increase offset 4 bytes to account for read
			of += 4;
		}

		for (auto x = 0; x < iV * 2; x += 2) {
			uint16_t val = data[of] | data[of + 1] << 8;
			ivV.emplace_back(val);
			// Increase offset 2 bytes to account for read
			of += 2;
		}

		for (auto x = 0; x < iT * 2; x += 2) {
			uint16_t val = data[of] | data[of + 1] << 8;
			ivT.emplace_back(val);
			// Increase offset 2 bytes to account for read
			of += 2;
		}

		for (auto x = 0; x < iN * 2; x += 2) {
			uint16_t val = data[of] | data[of + 1] << 8;
			ivN.emplace_back(val);
			// Increase offset 2 bytes to account for read
			of += 2;
		}

		vertOut.reserve(iV);
		uvOut.reserve(iT);
		normOut.reserve(iN);

		// Reconstruct model by iterating indices and copying associated verts / UVs / normals
		for (auto x = 0; x < iV; ++x) {
			auto start = (ivV[x] - 1) * 3;
			vertOut.emplace_back(vertices[start]);
			vertOut.emplace_back(vertices[start + 1]);
			vertOut.emplace_back(vertices[start + 2]);
		}

		for (auto x = 0; x < iT; ++x) {
			auto start = (ivT[x] - 1) * 2;
			uvOut.emplace_back(uvs[start]);
			uvOut.emplace_back(uvs[start + 1]);
		}

		for (auto x = 0; x < iN; ++x) {
			auto start = (ivN[x] - 1) * 3;
			normOut.emplace_back(vertices[start]);
			normOut.emplace_back(normals[start + 1]);
			normOut.emplace_back(normals[start + 2]);
		}

		return true;
	}
}