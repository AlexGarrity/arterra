# Block Data and Management

Blocks are separated into two parts - block data and the instance of the block.  The instance is stored within a chunk, providing a representation of a block within the world space.  The block data is stored in the block manager, and referenced by instances of that block.  

This approach means that each instance of a block only needs to store a reference to its data rather than the data itself, meaning greatly reduced memory usage.  Furthermore, it allows for modification of block data to effect every instance of it rather than having to iterate over all the blocks.  

Additionally, the block manager has the feature of mapping all the blocks.  This means that blocks can be serialised into just numbers, representing the block data they instance.  Upon load, this value will be converted to a reference to the required block data, provided by a lookup table storing the mapped `BlockData` structs.