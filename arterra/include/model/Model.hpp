#pragma once

#include "PCH.hpp"

#include "glad/glad.h"

#include "util/Resource.hpp"
#include "model/OBJ.hpp"

namespace arterra {

    class Model {
        struct ModelBuffers {
            enum class Vx { V, VT, VN, VTN };
            Vx _vertices = Vx::V;
            GLuint _vertexBuffer = 0;
            GLuint _vElementBuffer = 0;

            GLuint _uvBuffer = 0;
            GLuint _uElementBuffer = 0;

            GLuint _normalBuffer = 0;
            GLuint _nElementBuffer = 0;

            ~ModelBuffers() {
                switch (_vertices) {
                    case Vx::VTN:
                        glDeleteBuffers(1, &_normalBuffer);
                        glDeleteBuffers(1, &_nElementBuffer);
                    case Vx::VT:
                        glDeleteBuffers(1, &_uvBuffer);
                        glDeleteBuffers(1, &_uElementBuffer);
                        break;
                    case Vx::VN:
                        glDeleteBuffers(1, &_normalBuffer);
                        glDeleteBuffers(1, &_nElementBuffer);
                        break;
                };
                glDeleteBuffers(1, &_vertexBuffer);
                glDeleteBuffers(1, &_vElementBuffer);
            }
        };

        public:
            Model() {
                glGenBuffers(1, &_buffers._vElementBuffer);
                glGenBuffers(1, &_buffers._vertexBuffer);

                glGenBuffers(1, &_buffers._uElementBuffer);
                glGenBuffers(1, &_buffers._uvBuffer);

                glGenBuffers(1, &_buffers._nElementBuffer);
                glGenBuffers(1, &_buffers._normalBuffer);
            }

            Model(std::string filepath) {
                Create(filepath);
            }

            ~Model() {

            }

            void BindVertexData() {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers._vElementBuffer);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indicesV.size() * sizeof(float_t), _indicesV.data(), GL_STATIC_DRAW);

                glBindBuffer(GL_ARRAY_BUFFER, _buffers._vertexBuffer);
                glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), _vertices.data(), GL_STATIC_DRAW);
            }

            void BindUVData() {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers._uElementBuffer);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indicesT.size() * sizeof(float_t), _indicesT.data(), GL_STATIC_DRAW);

                glBindBuffer(GL_ARRAY_BUFFER, _buffers._uvBuffer);
                glBufferData(GL_ARRAY_BUFFER, _uvs.size() * sizeof(GLfloat), _uvs.data(), GL_STATIC_DRAW);
            }

            void BindNormalData() {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers._nElementBuffer);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indicesN.size() * sizeof(float_t), _indicesN.data(), GL_STATIC_DRAW);

                glBindBuffer(GL_ARRAY_BUFFER, _buffers._normalBuffer);
                glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(GLfloat), _normals.data(), GL_STATIC_DRAW);
            }


            bool Create(std::string filepath) {
                auto resource = Resource::Get().Load(filepath);
                if (!resource) {
                    Logger::Get().Log(Logger::Warning, "Failed to load model '", filepath, "'");
                    return false;
                }
                auto resourceHandle = Resource::Get().Get(filepath);

                auto objModel = OBJ { resourceHandle._resource->_data };
                objModel.FillVectors(_vertices, _uvs, _normals, _indicesV, _indicesT, _indicesN);

                BindVertexData();

                switch (0x0F & !_uvs.empty() | 0xF0 & _normals.empty()) {
                    case 0xFF:
                        _buffers._vertices = ModelBuffers::Vx::VTN;
                        BindNormalData();
                        BindUVData();
                        break;
                    case 0xF0:
                        _buffers._vertices = ModelBuffers::Vx::VN;
                        BindNormalData();
                        break;
                    case 0x0F:
                        _buffers._vertices = ModelBuffers::Vx::VT;
                        BindUVData();
                        break;
                    default:
                        _buffers._vertices = ModelBuffers::Vx::V;
                }   
                return true;             
            }


            void Render() {
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, _buffers._vertexBuffer);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, _buffers._uvBuffer);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

                glEnableVertexAttribArray(2);
                glBindBuffer(GL_ARRAY_BUFFER, _buffers._normalBuffer);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers._vElementBuffer);

                glDrawElements(GL_TRIANGLES, _indicesV.size(), GL_UNSIGNED_SHORT, _indicesV.data());

                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(2);
            }




        private:
            ModelBuffers _buffers;

            std::vector<float_t> _vertices;
            std::vector<float_t> _normals;
            std::vector<float_t> _uvs;

            std::vector<uint16_t> _indicesV;
            std::vector<uint16_t> _indicesT;
            std::vector<uint16_t> _indicesN;


    };


}