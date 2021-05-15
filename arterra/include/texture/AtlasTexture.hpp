#pragma once

namespace arterra {
	
	struct AtlasTexture {
		const float _width, _height;
		const float _x, _y;

		AtlasTexture(float w, float h, float x, float y);

		AtlasTexture();

		AtlasTexture(const AtlasTexture& other);

		AtlasTexture(size_t w, size_t h, size_t x, size_t y, size_t aW, size_t aH);
	};
	
}