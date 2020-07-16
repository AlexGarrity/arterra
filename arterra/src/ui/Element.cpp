#include "ui/Element.hpp"

namespace arterra {
	
	namespace UI {
		
		ElementMesh::ElementMesh()
			: _posVertices(), _uvVertices(), _vertexArray(), _posBuffer(), _texBuffer() {}
		
		ElementMesh::ElementMesh(std::vector<float_t> posVertices, std::vector<float_t> uvVertices)
			: _posVertices(posVertices), _uvVertices(uvVertices) {
			GenerateMesh();
		}
		
		Element::Element() 
			: _width(0), _height(0), _position(glm::vec2(0.0f, 0.0f)), _anchor(ElementAnchor::BottomLeft),
			_mesh(ElementMesh()), _material(Material()) {}
		
		Element::Element(int width, int height, glm::vec2 position, ElementAnchor anchor, AtlasTexture* texture,
			Material material)
			: _width(width), _height(height), _position(position), _anchor(anchor), _material(material) {
			
			// Reserve space for vertex position data.
			std::vector<float_t> positions;
			positions.resize(12);
			// Translate uv 0-1 coordinates to atlas coordinates.
			std::vector<float_t> uvs = {
				texture->_x, texture->_y,
				texture->_x + texture->_width, texture->_y,
				texture->_x, texture->_y + texture->_height,
				texture->_x, texture->_y + texture->_height,
				texture->_x + texture->_width, texture->_y,
				texture->_x + texture->_width, texture->_y + texture->_height
			};
			
			int halfWidth = width / 2;
			int halfHeight = height / 2;
			
			// Calculate the vertex positions based off the anchor point.
			switch (anchor)
			{
			case ElementAnchor::BottomLeft:
				positions = {
					position.x, position.y,
					position.x + width, position.y,
					position.x, position.y + height,
					position.x, position.y + height,
					position.x + width, position.y,
					position.x + width, position.y + height
				};
				break;
			case ElementAnchor::BottomRight:
				positions = {
					position.x - width, position.y,
					position.x, position.y,
					position.x - width, position.y + height,
					position.x - width, position.y + height,
					position.x, position.y,
					position.x, position.y + height
				};
				break;
			case ElementAnchor::BottomCentre:
				positions = {
					position.x - halfWidth, position.y,
					position.x + halfWidth, position.y,
					position.x - halfWidth, position.y + height,
					position.x - halfWidth, position.y + height,
					position.x + halfWidth, position.y,
					position.x + halfWidth, position.y + height
				};
				break;
			case ElementAnchor::TopLeft:
				positions = {
					position.x, position.y - height,
					position.x + width, position.y - height,
					position.x, position.y,
					position.x, position.y,
					position.x + width, position.y - height,
					position.x + width, position.y
				};
				break;
			case ElementAnchor::TopRight:
				positions = {
					position.x - width, position.y - height,
					position.x, position.y - height,
					position.x - width, position.y,
					position.x - width, position.y,
					position.x, position.y - height,
					position.x, position.y
				};
				break;
			case ElementAnchor::TopCentre:
				positions = {
					position.x - halfWidth, position.y - height,
					position.x + halfWidth, position.y - height,
					position.x - halfWidth, position.y,
					position.x - halfWidth, position.y,
					position.x + halfWidth, position.y - height,
					position.x + halfWidth, position.y
				};
				break;
			case ElementAnchor::Left:
				positions = {
					position.x, position.y - halfHeight,
					position.x + width, position.y - halfHeight,
					position.x, position.y + halfHeight,
					position.x, position.y + halfHeight,
					position.x + width, position.y - halfHeight,
					position.x + width, position.y + halfHeight
				};
				break;
			case ElementAnchor::Right:
				positions = {
					position.x - width, position.y - halfHeight,
					position.x, position.y - halfHeight,
					position.x - width, position.y + halfHeight,
					position.x - width, position.y + halfHeight,
					position.x, position.y - halfHeight,
					position.x, position.y + halfHeight
				};
				break;
			case ElementAnchor::Centre:
				positions = {
					position.x - halfWidth, position.y - halfHeight,
					position.x + halfWidth, position.y - halfHeight,
					position.x - halfWidth, position.y + halfHeight,
					position.x - halfWidth, position.y + halfHeight,
					position.x + halfWidth, position.y - halfHeight,
					position.x + halfWidth, position.y + halfHeight
				};
				break;
			}
			
			// Create the gui-element with the correct vertex data.
			_mesh = ElementMesh { positions, uvs };
			
		}
		
		Element::~Element() {
			
		}
		
	}
	
}