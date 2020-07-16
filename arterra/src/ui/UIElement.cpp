#include "ui/UIElement.hpp"

namespace arterra {
	
	UIElement::UIElement() 
		: _width(0), _height(0), _position(glm::vec2(0.0f, 0.0f)), _anchor(UIElementAnchor::BottomLeft) {}
	
	UIElement::UIElement(int width, int height, glm::vec2 position, UIElementAnchor anchor)
		: _width(width), _height(height), _position(position), _anchor(anchor) {
		
		// Calculate the vertices position based off the anchor point.
		std::vector<float_t> vertices;
		vertices.reserve(12);
		std::vector<float_t> uvs = {
			0.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			0.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 1.0f
		};
		int halfWidth = width / 2;
		int halfHeight = height / 2;
		
		switch (anchor)
		{
		case UIElementAnchor::BottomLeft:
			vertices = {
				position.x, position.y,
				position.x + width, position.y,
				position.x, position.y + height,
				position.x, position.y + height,
				position.x + width, position.y,
				position.x + width, position.y + height
			};
			break;
		case UIElementAnchor::BottomRight:
			vertices = {
				position.x - width, position.y,
				position.x, position.y,
				position.x - width, position.y + height,
				position.x - width, position.y + height,
				position.x, position.y,
				position.x, position.y + height
			};
			break;
		case UIElementAnchor::BottomCentre:
			vertices = {
				position.x - halfWidth, position.y,
				position.x + halfWidth, position.y,
				position.x - halfWidth, position.y + height,
				position.x - halfWidth, position.y + height,
				position.x + halfWidth, position.y,
				position.x + halfWidth, position.y + height
			};
			break;
		case UIElementAnchor::TopLeft:
			vertices = {
				position.x, position.y - height,
				position.x + width, position.y - height,
				position.x, position.y,
				position.x, position.y,
				position.x + width, position.y - height,
				position.x + width, position.y
			};
			break;
		case UIElementAnchor::TopRight:
			vertices = {
				position.x - width, position.y - height,
				position.x, position.y - height,
				position.x - width, position.y,
				position.x - width, position.y,
				position.x, position.y - height,
				position.x, position.y
			};
			break;
		case UIElementAnchor::TopCentre:
			vertices = {
				position.x - halfWidth, position.y - height,
				position.x + halfWidth, position.y - height,
				position.x - halfWidth, position.y,
				position.x - halfWidth, position.y,
				position.x + halfWidth, position.y - height,
				position.x + halfWidth, position.y
			};
			break;
		case UIElementAnchor::Left:
			vertices = {
				position.x, position.y - halfHeight,
				position.x + width, position.y - halfHeight,
				position.x, position.y + halfHeight,
				position.x, position.y + halfHeight,
				position.x + width, position.y - halfHeight,
				position.x + width, position.y + halfHeight
			};
			break;
		case UIElementAnchor::Right:
			vertices = {
				position.x - width, position.y - halfHeight,
				position.x, position.y - halfHeight,
				position.x - width, position.y + halfHeight,
				position.x - width, position.y + halfHeight,
				position.x, position.y - halfHeight,
				position.x, position.y + halfHeight
			};
			break;
		case UIElementAnchor::Centre:
			vertices = {
				position.x - halfWidth, position.y - halfHeight,
				position.x + halfWidth, position.y - halfHeight,
				position.x - halfWidth, position.y + halfHeight,
				position.x - halfWidth, position.y + halfHeight,
				position.x + halfWidth, position.y - halfHeight,
				position.x + halfWidth, position.y + halfHeight
			};
			break;
		}
		
		// Create gui element
		_guiElement = GuiElement { vertices, uvs };
		
	}
	
	
}