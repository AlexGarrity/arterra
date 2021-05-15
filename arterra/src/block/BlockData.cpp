#include "block/BlockData.hpp"

#include "model/CullableModel.hpp"

namespace arterra {

	BlockData::BlockData(const AtlasTexture& topTexture, const AtlasTexture& sideTexture,
		const AtlasTexture& bottomTexture, const CullableModel& model)
		: _topTexture { topTexture }
		, _sideTexture { sideTexture }
		, _bottomTexture { bottomTexture }
		, _model { model }
	{
	}

	BlockData::BlockData(const BlockData& other)
		: _topTexture { other._topTexture }
		, _sideTexture { other._sideTexture }
		, _bottomTexture { other._bottomTexture }
		, _model { other._model }
		, _cullingSettings { other._cullingSettings }
	{
	}

	BlockData::BlockData(const BlockData&& other)
		: _topTexture { other._topTexture }
		, _sideTexture { other._sideTexture }
		, _bottomTexture { other._bottomTexture }
		, _model { other._model }
		, _cullingSettings { other._cullingSettings }
	{
	}

	const AtlasTexture& BlockData::GetTexture(const Direction direction) const
	{
		switch (direction) {
			case Direction::PosY:
				return _topTexture;
			case Direction::NegY:
				return _bottomTexture;
			default:
				break;
		}
		return _sideTexture;
	}


}