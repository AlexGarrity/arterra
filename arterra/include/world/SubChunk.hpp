#pragma once

#include "PCH.hpp"
#include "world/Block.hpp"

namespace arterra {

	class Chunk;

	class SubChunk {

	public:
		// Static consts define the physical constraints of a subchunk
		// 16 or 32 are ideal values
		static const int SIZE_X = 16;
		static const int SIZE_Y = 16;
		static const int SIZE_Z = 16;
		static const int SIZE = SIZE_X * SIZE_Y * SIZE_Z;

		/**
		 * @brief Default constructor for SubChunk
		 *
		 * @param posY The Y coordinate of this SubChunk in the chunk space
		 * @param parent A pointer to the chunk this SubChunk is a child of
		 **/
		SubChunk(int posY, Chunk* parent);
		/**
		 * @brief Copy constructor for SubChunk
		 * @param other A SubChunk to copy the properties of
		 **/
		SubChunk(const SubChunk& other);
		/**
		 * @brief Destructor for SubChunk
		 **/
		~SubChunk();

		/**
		 * @brief Set the parent chunk of this SubChunk
		 * @param chunk A pointer to the Chunk this SubChunk should be a child of
		 **/
		void SetParent(Chunk* chunk);

		/**
		 * @brief Get the blocks contained in this SubChunk
		 * @return An array containing pointers to all the blocks in this SubChunk
		 * */
		std::array<Block*, SIZE>& GetBlocks();

		/**
		 * @brief Get a pointer to the block at the given coordinates
		 *
		 * @param x The X coordinate of the block in the world space
		 * @param y The Y coordinate of the block in the world space
		 * @param z The Z coordinate of the block in the world space
		 *
		 * @return If x,y,z is a valid block, return a pointer to the block at that location.
		 * If x,y,z, is not valid, return nullptr
		 **/
		Block* GetBlock(int x, int y, int z) const;
		/**
		 * @brief Set the block at the given coordinates
		 *
		 * @param x The X coordinate of the block in the world space
		 * @param y The Y coordinate of the block in the world space
		 * @param z The Z coordinate of the block in the world space
		 * @param data A reference to the BlockData struct the block should represent
		 **/
		void SetBlock(int x, int y, int z, BlockData& data);
		/**
		 * @brief Delete the block at the given coordinates
		 *
		 * @param x The X coordinate of the block in the world space
		 * @param y The Y coordinate of the block in the world space
		 * @param z The Z coordinate of the block in the world space
		 **/
		void DeleteBlock(int x, int y, int z);

		/**
		 * @brief Get a pointer to the block at the given coordinates
		 *
		 * @param x The X coordinate of the block in the chunk space
		 * @param y The Y coordinate of the block in the chunk space
		 * @param z The Z coordinate of the block in the chunk space
		 *
		 * @return If x,y,z is a valid block, return a pointer to the block at that location.
		 * If x,y,z, is not valid, return nullptr
		 **/
		Block* GetBlockCS(int x, int y, int z) const;
		/**
		 * @brief Set the block at the given coordinates
		 *
		 * @param x The X coordinate of the block in the chunk space
		 * @param y The Y coordinate of the block in the chunk space
		 * @param z The Z coordinate of the block in the chunk space
		 * @param data A reference to the BlockData struct the block should represent
		 **/
		void SetBlockCS(int x, int y, int z, BlockData& data);
		/**
		 * @brief Delete the block at the given coordinates
		 *
		 * @param x The X coordinate of the block in the chunk space
		 * @param y The Y coordinate of the block in the chunk space
		 * @param z The Z coordinate of the block in the chunk space
		 **/
		void DeleteBlockCS(int x, int y, int z);

		/**
		 * @brief Update all the blocks along a given border of the SubChunk
		 * @param borderDirection A value of the Direction enum representing the border to update
		 **/
		void UpdateBorder(Direction borderDirection);

		/**
		 * @brief Get the position of the SubChunk in the world space
		 * @return A WorldPosition struct representing the position of the SubChunk in the world space
		 **/
		WorldPosition GetPosition();
		/**
		 * @brief Get the position of the SubChunk in the chunk space
		 * @return An integer representing the Y coordinate of the SubChunk in the chunk space
		 **/
		int GetPositionRaw();
		/**
		 * @brief Get the parent Chunk of this SubChunk
		 * @return A pointer to the Chunk this SubChunk is a child of
		 **/
		inline Chunk* GetChunk() { return _chunk; };

		/**
		 * @brief Update this SubChunk
		 * @param deltaTime A float representing amount of time since the engine last updated
		 * @return A bool representing whether or not any blocks in this SubChunk have been changed
		 **/
		bool Update(float deltaTime);

	private:
		// Y coordinate of the subchunk (in CS)
		int _position;
		// Parent chunk
		Chunk* _chunk;
		// Whether or not a block in the chunk has been changed (recalc mesh)
		bool _updated = true;

		// Contains SIZE_X * SIZE_Y * SIZE_Z blocks.
		std::array<Block*, SIZE> _blocks;
	};

}