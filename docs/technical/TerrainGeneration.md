# Terrain Generation

Terrain generation is implemented using GLM's Simplex noise functionality.  
Terrain is generated in two individual parts.  Firstly, a height map is generated using solely the X and Z coordinates of a block within a chunk.  This heightmap is calculated on a per-chunk basis, adding the chunk coordinates to keep the world smooth.  

The second phase uses 3D noise as opposed to 2D to generate cave-like structures.  This is done on a per-block basis, and can override what the heightmap has done.  

Terrain is scaled down to 64x scale, multiplying all coordinates by a factor of `0.015625` (`1/64`).  This ensures that terrain is smooth whilst also having noticeable gradients.  This value should be tweakable during generation, although will be consistent across a whole world to ensure that terrain has no artifacting.