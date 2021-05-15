#include "world/Block.hpp"

#include "world/Chunk.hpp"
#include "world/SubChunk.hpp"
#include "world/World.hpp"

namespace arterra {

	Block::Block(const Block& other)
		: _subChunk(other._subChunk)
		, _position(other.GetPositionRaw())
		, _blockData(other._blockData)
		, _world(other._world)
	{
	}

	Block::Block(int posX, int posY, int posZ, SubChunk* subChunk, BlockData& blockData)
		: _position(posX, posY, posZ)
		, _subChunk(subChunk)
		, _blockData(blockData)
		, _visibleFaces { false }
	{
		_world = _subChunk->GetChunk()->GetWorld();
	}

	WorldPosition Block::GetPosition() const
	{
		auto scPosition = _subChunk->GetPosition();
		return WorldPosition { _position._x + scPosition._x, _position._y + scPosition._y,
			_position._z + scPosition._z };
	}

	WorldPosition Block::GetPositionRaw() const { return { _position._x, _position._y, _position._z }; }

	BlockData& Block::GetData() const { return _blockData; }
	
	bool Block::IsVisible() const { return _visible; }

	std::array<bool, 6> Block::GetVisibleFaces() const { return _visibleFaces; }

	void Block::Update(size_t width)
	{
		auto neighbours = GetNeighbours();
		UpdateVisiblity(neighbours);

		if (width != 0) {
			for (auto n : neighbours) {
				if (!n)
					continue;
				n->Update(width - 1);
			}
		}
	}

	void Block::UpdateVisiblity(std::array<Block*, 6>& neighbours)
	{
		_visible = false;
		for (auto i = 0; i < 6; ++i) {
			bool blockExists = (neighbours[i] != nullptr);
			if (!blockExists) {
				_visibleFaces[i] = true;
				_visible = true;
			} else {
				auto &sCS = _blockData.GetCullingSettings();
				auto &bd = neighbours[i]->GetData();
				auto &oCS = bd.GetCullingSettings();
				switch (sCS._cullingMode) {
					case CullingSettings::Always: {
						_visibleFaces[i] = false;
					} break;
					case CullingSettings::SameBlock: {
						bool getsCulled = (sCS._getsCulled[i] & oCS._cullsOther[i]);
						bool sameBlock = (&_blockData == &neighbours[i]->GetData());
						_visibleFaces[i] = (getsCulled & sameBlock);
						_visible = (getsCulled & sameBlock);
					} break;
					case CullingSettings::SameModel: {
						bool getsCulled = (sCS._getsCulled[i] & oCS._cullsOther[i]);
						bool sameModel = (&_blockData.GetModel() == &neighbours[i]->GetData().GetModel());
						_visibleFaces[i] = (getsCulled & sameModel);
						_visible = (getsCulled & sameModel);
					}
					case CullingSettings::Never: {
						_visibleFaces[i] = true;
						_visible = true;
					} break;
				}
			}
		}
	}

	Block* Block::ResolveBlockPositionOptimised(const int v, const int axis, const int oA, const int oX,
		const int oY, const int oZ, const WorldPosition &cpos, const WorldPosition &pos) const
	{
		// Resolve for x + 1
		if ((v + oA > axis) || (v + oA < 0)) {
			return _world->GetBlock(pos._x + oX, pos._y + oY, pos._z + oZ);
		} else {
			return _subChunk->GetBlockCS(cpos._x + oX, cpos._y + oY, cpos._z + oZ);
		}
	}

	std::array<Block*, 6> Block::GetNeighbours()
	{
		std::array<Block*, 6> b { nullptr };

		const auto pos = GetPosition();
		const auto cpos = GetPositionRaw();

		// Resolve for x
		b[0] = ResolveBlockPositionOptimised(cpos._x, SubChunk::SIZE_X, 1, 1, 0, 0, cpos, pos);
		b[1] = ResolveBlockPositionOptimised(cpos._x, SubChunk::SIZE_X, -1, -1, 0, 0, cpos, pos);

		// Resolve for y
		b[2] = ResolveBlockPositionOptimised(cpos._y, SubChunk::SIZE_Y, 1, 0, 1, 0, cpos, pos);
		b[3] = ResolveBlockPositionOptimised(cpos._y, SubChunk::SIZE_Y, -1, 0, -1, 0, cpos, pos);

		// Resolve for z
		b[4] = ResolveBlockPositionOptimised(cpos._z, SubChunk::SIZE_Z, 1, 0, 0, 1, cpos, pos);
		b[5] = ResolveBlockPositionOptimised(cpos._z, SubChunk::SIZE_Z, -1, 0, 0, -1, cpos, pos);

		return b;
	}

	void Block::SetParent(SubChunk* subChunk)
	{
		_subChunk = subChunk;
		_world = _subChunk->GetChunk()->GetWorld();
	}

}