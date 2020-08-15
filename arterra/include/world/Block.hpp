#pragma once

#include "PCH.hpp"
#include "block/BlockData.hpp"
#include "world/WorldPosition.hpp"

namespace arterra {

	class World;
	class SubChunk;

	class Block {

	public:
		/**
		 * @brief Copy constructor for Block
		 * @param other Another Block to copy the properties of
		 **/
		Block(const Block& other);

		/**
		 * @brief Default constructor for Block
		 *
		 * @param posX The X coordinate of the block in the chunk space
		 * @param posY The Y coordinate of the block in the chunk space
		 * @param posZ The Z coordinate of the block in the chunk space
		 * @param subChunk A pointer to the parent subchunk of this block
		 * @param blockData A reference to the BlockData struct this block should represent
		 **/
		Block(int posX, int posY, int posZ, SubChunk* subChunk, BlockData& blockData);

		/**
		 * @brief Get the position of the block in the world space
		 * @return A WorldPosition struct representing the position of the block in the world space
		 **/
		WorldPosition GetPosition() const;
		/**
		 * @brief Get the position of the block in the chunk space
		 * @return A WorldPosition struct representing the position of the block in the chunk space
		 **/
		WorldPosition GetPositionRaw() const;
		/**
		 * @brief Get a reference to the data which this block represents
		 * @return A const reference to the BlockData struct this block represents
		 **/
		BlockData& GetData() const;

		// Return whether or not the block is visible
		inline bool IsVisible() const { return _visible; }

		/**
		 * @brief Get a list of the visible faces, in order of the direction enum
		 * @return A const array, size 6, of booleans representing wether a face is visible or not
		 **/
		std::array<bool, 6> GetVisibleFaces() const;

		/**
		 * @brief Update this block, rechecking whether it's faces are visible and optionally updating neighbours
		 * @param width A size_t representing how far this update should travel.  Zero will update just this block,
		 * one will update this and the blocks next to it, etc.
		 **/
		void Update(size_t width);
		/**
		 * @brief Update the visibility of this block, checking which faces are visible
		 * @param neighbours A reference to the array of pointers representing the neighbours of this block
		 **/
		void UpdateVisiblity(std::array<Block*, 6>& neighbours);
		/**
		 * @brief Get an array of pointers to the blocks surrounding this block
		 * @return An array, size 6, of pointers to the blocks immediately surrounding this block
		 **/
		std::array<Block*, 6> GetNeighbours();

		/**
		 * @brief Set the parent subchunk of this block
		 * @param subChunk The subchunk to parent this block to
		 **/
		void SetParent(SubChunk* subChunk);

	private:
		bool _visible;
		std::array<bool, 6> _visibleFaces;

		BlockData& _blockData;
		SubChunk* _subChunk;
		World* _world;
		WorldPosition _position;
	};

}