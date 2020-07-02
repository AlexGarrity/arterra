#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>
#include <cmath>

namespace arterra {
    class OBJ {
        protected:
            std::vector<float_t> _vertices {};
            std::vector<float_t> _normals {};
            std::vector<float_t> _uvs {};


        public:
            OBJ() = default;

            OBJ(std::vector<uint8_t> data)
            {
                // Check that there's at least enough data for the header
                if (data.size() < 16) {
                    // Data is too small
                    return;
                }

                // Check we have the right data
                auto magicNum = std::string (data.begin(), data.begin() + 4);
                if (magicNum != "MOBJ") {
                    // Wrong data, I guess
                    return;
                }

                uint16_t vertexLength = 0;
                uint16_t textureLength = 0;
                uint16_t normalCount = 0;
                uint16_t iV, iT, iN;

                // Declare a load of vectors
                std::vector<uint16_t> ivV, ivT, ivN;
                std::vector<float_t> vertices, uvs, normals;

                // Extract relevant data from the header
                vertexLength = static_cast<uint16_t>(data[4]) | static_cast<uint16_t>(data[5]) << 8;
                textureLength = static_cast<uint16_t>(data[6]) | static_cast<uint16_t>(data[7]) << 8;
                normalCount = static_cast<uint16_t>(data[8]) | static_cast<uint16_t>(data[9]) << 8;
                
                iV = static_cast<uint16_t>(data[10]) | static_cast<uint16_t>(data[11]) << 8;
                iT = static_cast<uint16_t>(data[12]) | static_cast<uint16_t>(data[13]) << 8;
                iN = static_cast<uint16_t>(data[14]) | static_cast<uint16_t>(data[15]) << 8;

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
                    uint32_t val = data[of] | data[of+1] << 8 | data[of+2] << 16 | data[of+3] << 24;
                    float_t fVal = *reinterpret_cast<float_t*>(&val);
                    vertices.emplace_back(fVal);
                    // Increase offset 4 bytes to account for read
                    of += 4;
                }

                for (auto x = 0; x < textureLength * 4; x += 4) {
                    uint32_t val = data[of] | data[of+1] << 8 | data[of+2] << 16 | data[of+3] << 24;
                    float_t fVal = *reinterpret_cast<float_t*>(&val);
                    uvs.emplace_back(fVal);
                    // Increase offset 4 bytes to account for read
                    of += 4;
                }

                for (auto x = 0; x < normalCount * 4; x += 4) {
                    uint32_t val = data[of] | data[of+1] << 8 | data[of+2] << 16 | data[of+3] << 24;
                    float_t fVal = *reinterpret_cast<float_t*>(&val);
                    normals.emplace_back(fVal);
                    // Increase offset 4 bytes to account for read
                    of += 4;
                }

                for (auto x = 0; x < iV * 2; x += 2) {
                    uint16_t val = data[of] | data[of+1] << 8;
                    ivV.emplace_back(val);
                    // Increase offset 2 bytes to account for read
                    of += 2;
                }

                for (auto x = 0; x < iT * 2; x += 2) {
                    uint16_t val = data[of] | data[of+1] << 8;
                    ivT.emplace_back(val);
                    // Increase offset 2 bytes to account for read
                    of += 2;
                }
                
                for (auto x = 0; x < iN * 2; x += 2) {
                    uint16_t val = data[of] | data[of+1] << 8;
                    ivN.emplace_back(val);
                    // Increase offset 2 bytes to account for read
                    of += 2;
                }

                // Reconstruct model
                for (auto x = 0; x < iV; ++x) {
                    auto start = (ivV[x] - 1) * 3;
                    _vertices.emplace_back(vertices[start]);
                    _vertices.emplace_back(vertices[start+1]);
                    _vertices.emplace_back(vertices[start+2]);
                
                }

                for (auto x = 0; x < iT; ++x) {
                    auto start = (ivT[x] - 1) * 2;
                    _uvs.emplace_back(uvs[start]);
                    _uvs.emplace_back(uvs[start+1]);
                }

                for (auto x = 0; x < iN; ++x) {
                    auto start = (ivN[x] - 1) * 3;
                    _normals.emplace_back(vertices[start]);
                    _normals.emplace_back(normals[start+1]);
                    _normals.emplace_back(normals[start+2]);
                }
            }

            inline std::vector<float_t> &Vertices() { return _vertices; }
            inline std::vector<float_t> &Normals() { return _normals; }
            inline std::vector<float_t> &UVs() { return _uvs; }

            size_t Size() const { return 
                (
                    _vertices.size() * sizeof(float_t) + 
                    _normals.size() * sizeof(float_t) +
                    _uvs.size() * sizeof(float_t) +
                    sizeof(OBJ)
                );
            }
    };

}