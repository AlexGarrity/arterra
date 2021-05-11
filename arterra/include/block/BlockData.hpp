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
		BlockData(const AtlasTexture& top, const AtlasTexture& side, const AtlasTexture& bottom, const CullableModel& model);
		BlockData(const BlockData &other);
		BlockData(const BlockData &&other);

		void operator=(const BlockData &other) = delete;

		const CullableModel& GetModel() const { return _model; }
		const AtlasTexture& GetTexture(Direction direction) const;
		const CullingSettings GetCullingSettings() const { return _cullingSettings; }

	private:
		const AtlasTexture& _topTexture;
		const AtlasTexture& _sideTexture;
		const AtlasTexture& _bottomTexture;

		const CullableModel& _model;
		const CullingSettings _cullingSettings;
	};

}