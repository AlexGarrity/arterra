# World Data/Design
The design and data ideas for all things in-game world related.

## Chunks
A chunk is `16x16x256` blocks, with subchunks of `16x16x16` size. Frustum culling will work on a subchunk basis.

Each subchunk is a `vector<block>` containing `4096` blocks.
A chunk is made up of a `vector<vector<block>>` which contains 16 subchunks, for a total `65536` blocks.

The chunk data is stored from the bottom up, along the x-axis first, then along the z-axis.
Block data will be serialised in this format:
`(<header>(number of bytes) <block id> <block properties>)`.
Block position data is serialised from a `float` into `4 bits` to save storage space.
Chunk data will be grouped into one file, containing `???` chunks per file.

Once per world, a translation table will be written to translate `block id` to `block name`.



## Block


