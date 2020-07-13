# World Data/Design
The design and data ideas for general in-game world related things.

## Chunks
A chunk is comprised of a potentially infinite quantity of subchunks, which are of `16*16*16` size. Frustum culling works on a subchunk basis.  The sizes of both of these classes can be changed at compile time, as they are defined as const values, and the codebase uses these rather than hard-defined constants.

Each subchunk is an `array<Block*>` containing `4096` (`16*16*16`) blocks.
A chunk is made up of an `unordered_map<BlockPosition, SubChunk>` which can contain a potentially infinite number of subchunks vertically, but will only store the data for subchunks in use.  Theoretically, a floating island could exist 10000 blocks above the surface and it would only use the surface plus a few more subchunks worth of storage.

The chunk data is stored from the bottom up, along the x-axis first, then along the z-axis.
Block data will be serialised in order, with zero representing air or any other value being a mapped block data struct.  Position data is not saved as it will be read linearly
The general storage format for a chunk will therefore be:
```
Chunk Header (indicates that this block is a chunk)
Chunk Data (subchunk count, etc..)
Chunk SubChunks (contains all subchunks)
```

Each SubChunk will be serialised as follows:
```
SubChunk header (indicates that this block is a subchunk)
SubChunk Data (position, biome, etc...)
Blocks (mapped block ID's, stored as integers, always 4096)
```

Chunks will be stored as individual files, as they could potentially be hundreds of subchunks tall or quite small

Once per world, the block mapping table will be written to the top of the file.  This will follow roughly the following format:
```
BlockMapper header (indicates that this block is the translation table)
BlockData count (the number of block data structs in the world)
BlockName:BlockID (the mapping of a string ID to an integer ID)
```



