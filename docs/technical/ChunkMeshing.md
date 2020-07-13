# Chunk Meshing

Chunk meshing works by iterating over every block in a chunk and checking for its neighbours.  If it has no
neighbour on any side, the block is marked as visible and that side will be drawn.  If it has neighbours on every
side, the block is marked as not visible, and no faces will be drawn.  

These are converted into a mesh by adding the relevant vertices from the model into a vertex buffer for the
specific chunk.  This is the only data a chunk mesh retains, as the vertice list will be deleted to save memory.  

When the chunk mesh is to be drawn, it can be bound and drawn using the draw triangles function of the renderer.  

Chunk meshing should ideally use greedy meshing at some point in the future, and draw using elements rather than repeating points, but this will require additional research for the time being as well as modification of the model format.