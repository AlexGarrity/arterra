#pragma once

#include "PCH.hpp"

#include "gamestate/Base.hpp"
#include "model/Model.hpp"
#include "world/Block.hpp"
#include "world/SubChunk.hpp"

namespace arterra {

    namespace gamestate {

        class Test : public Base {
        public:
            Test(Engine *engine);

            virtual void Input(float_t deltaTime) override;
            virtual void Update(float_t deltaTime) override;
            virtual void Render(float_t deltaTime) override;

            private:
                float_t _timeToResourceUnload = 5.0f;
				CullableModel _cubeModel;
				SubChunk _subChunk;
				
				ShaderManager _shaderManager;
				
				VertexArray vao;
				VertexBuffer vbo;
				std::vector<float_t> _meshVertices;

        };

    }

}