#include "block/BlockData.hpp"

namespace arterra {

	BlockData::BlockData(
		AtlasTexture& topTexture, AtlasTexture& sideTexture, AtlasTexture& bottomTexture, CullableModel& model)
		: _topTexture { topTexture }
		, _sideTexture { sideTexture }
		, _bottomTexture { bottomTexture }
		, _model { model }
	{
	}

	void BlockData::operator=(const BlockData& other)
	{
		_topTexture = other._topTexture;
		_sideTexture = other._sideTexture;
		_bottomTexture = other._bottomTexture;
		_model = other._model;
	}

	CullableModel& BlockData::GetModel() const { return _model; }

	AtlasTexture& BlockData::GetTexture(Direction direction) const {
		switch (direction) {
			case Direction::PosY:
				return _topTexture;
			case Direction::NegY:
				return _bottomTexture;
			default:
				return _sideTexture;
		}
	}

	CullingSettings BlockData::GetCullingSettings() const {
		return _cullingSettings;
	}

}