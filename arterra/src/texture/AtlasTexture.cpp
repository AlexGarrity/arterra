#include "texture/AtlasTexture.hpp"

namespace arterra {


	AtlasTexture::AtlasTexture(float w, float h, float x, float y)
		: _width{ w }
		  , _height{ h }
		  , _x{ x }
		  , _y{ y } { }

	AtlasTexture::AtlasTexture()
		: AtlasTexture(0.0f, 0.0f, 0.0f, 0.0f) { }

	AtlasTexture::AtlasTexture(const AtlasTexture &other) :
	_width { other._width }
	, _height {other._height }
	, _x {other._x }
	, _y {other._y }
	{
		
	}

	AtlasTexture::AtlasTexture(const size_t w, const size_t h, const size_t x, const size_t y, const size_t aW, const size_t aH)
		: _width{ static_cast<float>(w) / static_cast<float>(aW) }
		  , _height{ static_cast<float>(h) / static_cast<float>(aH) }
		  , _x{ static_cast<float>(x) / static_cast<float>(aW) }
		  , _y{ static_cast<float>(y) / static_cast<float>(aH) } { }
}
