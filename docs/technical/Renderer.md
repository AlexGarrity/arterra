# Renderer Architecture

The renderer is designed in a modular fashion such that it can be replaced with a different API by simply
inheriting it and writing a new API.  

It provides access to primitive functions like drawing triangles or lines.  These functions are later used by the
other renderers.  The chunk renderer, for example, converts chunks into meshes then draws them using the draw
triangles function of the base render.  This method is API-agnostic, and can therefore be kept neatly separated.