#pragma once

#include "PCH.hpp"

#include <glad/glad.h>

#include "model/CullableModel.hpp"
#include "texture/TextureAtlas.hpp"

namespace arterra {

	class BlockData {
	public:
		BlockData(AtlasTexture& top, AtlasTexture& side, AtlasTexture& bottom, CullableModel& model);

		void operator=(const BlockData &other);

		CullableModel &GetModel() const;
		AtlasTexture &GetTexture(Direction direction) const;

	private:
		AtlasTexture &_topTexture;
		AtlasTexture &_sideTexture;
		AtlasTexture &_bottomTexture;

		CullableModel& _model;
	};

}