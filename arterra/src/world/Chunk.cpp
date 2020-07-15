#include "world/Chunk.hpp"

#include "world/World.hpp"

namespace arterra {

	Chunk::Chunk(int posX, int posZ, World* world)
		: _posX { posX }
		, _posZ { posZ }
		, _world { world }
	{
	}

	Chunk::Chunk(const Chunk& other)
	{
		_posX = other._posX;
		_posZ = other._posZ;
		_world = other._world;
		_subChunks = std::move(other._subChunks);
		for (auto& sc : _subChunks) {
			sc.second.SetParent(this);
		}
	}

	void Chunk::CreateSubChunk(int y) { CreateSubChunkCS(y / SubChunk::SIZE_Y); }

	void Chunk::CreateSubChunksToHeight(int height) { CreateSubChunksToHeightCS(height / SubChunk::SIZE_Y); }

	SubChunk* Chunk::GetSubChunk(int y) { return GetSubChunkCS(y / SubChunk::SIZE_Y); }

	void Chunk::CreateSubChunkCS(int y)
	{
		if (_subChunks.find(y) != _subChunks.end())
			return;
		_subChunks.emplace(y, SubChunk(y, this));
	}

	void Chunk::CreateSubChunksToHeightCS(int height)
	{
		for (auto iY = 0; iY <= height; ++iY) {
			_subChunks.emplace(iY, SubChunk { iY, this });
		}
	}

	SubChunk* Chunk::GetSubChunkCS(int y)
	{
		auto it = _subChunks.find(y);
		if (it == _subChunks.end())
			return nullptr;
		return &it->second;
	}

	void Chunk::UpdateNeighbours()
	{
		Chunk* n;
		n = _world->GetChunkCS(_posX + 1, _posZ);
		if (n)
			n->UpdateBorder(Direction::NegX);
		n = _world->GetChunkCS(_posX - 1, _posZ);
		if (n)
			n->UpdateBorder(Direction::PosX);
		n = _world->GetChunkCS(_posX, _posZ + 1);
		if (n)
			n->UpdateBorder(Direction::NegZ);
		n = _world->GetChunkCS(_posX, _posZ - 1);
		if (n)
			n->UpdateBorder(Direction::PosZ);
	}

	void Chunk::UpdateBlocks() {
		for (auto &sc : _subChunks) {
			for (auto &b : sc.second.GetBlocks()) {
				if (b) b->Update(0);
			}
		}
	}

	void Chunk::UpdateBorder(Direction borderDirection)
	{
		switch (borderDirection) {
			case Direction::PosY: {
				Logger::Get().Log(Logger::Debug, "Updating positive Y border for chunk at (", _posX, ", ", _posZ,
					").  Whilst possible, was this intended?");
				SubChunkMap::iterator highestSC;
				for (auto it = _subChunks.begin(); it != _subChunks.end(); ++it) {
					if (it->second.GetPositionRaw() > highestSC->second.GetPositionRaw()) {
						highestSC = it;
					}
				}
				highestSC->second.UpdateBorder(PosY);
			} break;

			case Direction::NegY: {
				Logger::Get().Log(Logger::Debug, "Updating negative Y border for chunk at (", _posX, ", ", _posZ,
					").  Whilst possible, was this intended?");
				SubChunkMap::iterator lowestSC;
				for (auto it = _subChunks.begin(); it != _subChunks.end(); ++it) {
					if (it->second.GetPositionRaw() > lowestSC->second.GetPositionRaw()) {
						lowestSC = it;
					}
				}
				lowestSC->second.UpdateBorder(PosY);
			} break;

			default:
				for (auto& sc : _subChunks) {
					sc.second.UpdateBorder(borderDirection);
				}
				break;
		}
	}

	void Chunk::UpdateSubChunkBorder(int y, Direction borderDirection)
	{
		int scY = y / SubChunk::SIZE_Y;
		UpdateSubChunkBorderSC(scY, borderDirection);
	}

	void Chunk::UpdateSubChunkBorderSC(int y, Direction borderDirection)
	{
		auto it = _subChunks.find(y);
		if (it == _subChunks.end())
			return;
		it->second.UpdateBorder(borderDirection);
	}

	SubChunkMap& Chunk::GetSubChunks() { return _subChunks; }

	WorldPosition Chunk::GetPosition() { return { _posX * SubChunk::SIZE_X, 0, _posZ * SubChunk::SIZE_Z }; }

	std::vector<SubChunk*> Chunk::Update(float deltaTime)
	{
		std::vector<SubChunk*> out;
		out.reserve(4);
		for (auto& sc : _subChunks) {
			if (sc.second.Update(deltaTime)) {
				out.push_back(&sc.second);
			}
		}
		return { out.begin(), out.end() };
	}

}