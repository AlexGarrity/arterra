#pragma once

#include "PCH.hpp"

#include "gamestate/Base.hpp"
#include "model/Model.hpp"
#include "renderer/ChunkRenderer.hpp"
#include "world/World.hpp"

namespace arterra {

	namespace gamestate {

		class Test : public Base {
		public:
			Test(Engine* engine);

			virtual void Input(float_t deltaTime) override;
			virtual void Update(float_t deltaTime) override;
			virtual void Render(float_t deltaTime) override;

		private:
			float_t _timeToResourceUnload = 5.0f;
			CullableModel _cubeModel;
			World _world;

			ChunkRenderer _chunkRenderer;

			ShaderManager _shaderManager;
		};

	}

}