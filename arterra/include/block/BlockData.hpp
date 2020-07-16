#pragma once

#include "PCH.hpp"

#include <glad/glad.h>

#include "model/CullableModel.hpp"
#include "texture/TextureAtlas.hpp"

namespace arterra {

	class BlockData;

	struct CullingSettings {
		enum Mode { Always, SameModel, SameBlock, Never };
		std::array<bool, 6> _cullsOther { true, true, true, true, true, true };
		std::array<bool, 6> _getsCulled { true, true, true, true, true, true };
		Mode _cullingMode { CullingSettings::Always };
	};

	class BlockData {
	public:
		BlockData(AtlasTexture& top, AtlasTexture& side, AtlasTexture& bottom, CullableModel& model);

		void operator=(const BlockData& other);

		CullableModel& GetModel() const;
		AtlasTexture& GetTexture(Direction direction) const;
		CullingSettings GetCullingSettings() const;

	private:
		AtlasTexture& _topTexture;
		AtlasTexture& _sideTexture;
		AtlasTexture& _bottomTexture;

		CullableModel& _model;
		CullingSettings _cullingSettings;
	};

}