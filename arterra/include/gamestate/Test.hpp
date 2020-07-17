#pragma once

#include "PCH.hpp"

#include "block/BlockManager.hpp"
#include "gamestate/Base.hpp"
#include "renderer/ChunkRenderer.hpp"
#include "renderer/ShaderManager.hpp"
#include "ui/Element.hpp"
#include "ui/Material.hpp"
#include "ui/Manager.hpp"
#include "texture/TextureAtlas.hpp"
#include "texture/Texture.hpp"
#include "world/TerrainGenerator.hpp"
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
			CullableModel _blockModel;
			World _world;

			ChunkRenderer _chunkRenderer;
			ShaderManager _shaderManager;
			TerrainGenerator _terrainGenerator;
			BlockManager _blockManager;
			TextureAtlas _atlas;
			TextureAtlas _guiAtlas;
			
			// Temporary player controller.
			float_t _speed = 2.0f;
			float_t _rotSpeed = glm::radians(15.0f);
			
			// Temporary gui
			UI::Manager _uiManager;
			UI::Element _element1;
			UI::Element _element2;
			UI::Element _element3;
			//UI::Element _element2;
			UI::Material _mat1;
			UI::Material _mat2;
			UI::Material _mat3;
			AtlasTexture* _guiTexture;
			
		};

	}

}