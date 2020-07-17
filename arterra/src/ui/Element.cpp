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
			: _width(width), _height(height), _position(position), _anchor(anchor), _material(material),
				_texture(texture) {
		}
				
		void Element::Move(glm::vec2 movementVector) {
			_position.x += movementVector.x;
			_position.y += movementVector.y;
			CreateMesh();
			
		}
		
		void Element::Scale(float_t scaleFactor) {
			_width += scaleFactor;
			_height += scaleFactor;
			CreateMesh();
		}
		
		void Element::CreateMesh() {
			// Reserve space for vertex position data.
			std::vector<float_t> positions;
			positions.resize(12);
			// Translate uv 0-1 coordinates to atlas coordinates.
			std::vector<float_t> uvs = {
				_texture->_x, _texture->_y,
				_texture->_x + _texture->_width, _texture->_y,
				_texture->_x, _texture->_y + _texture->_height,
				_texture->_x, _texture->_y + _texture->_height,
				_texture->_x + _texture->_width, _texture->_y,
				_texture->_x + _texture->_width, _texture->_y + _texture->_height
			};
			
			int halfwidth = _width / 2;
			int halfheight = _height / 2;
			
			// Calculate the vertex positions based off the anchor point.
			switch (_anchor)
			{
			case ElementAnchor::BottomLeft:
				positions = {
					_position.x, _position.y,
					_position.x + _width, _position.y,
					_position.x, _position.y + _height,
					_position.x, _position.y + _height,
					_position.x + _width, _position.y,
					_position.x + _width, _position.y + _height
				};
				break;
			case ElementAnchor::BottomRight:
				positions = {
					_position.x - _width, _position.y,
					_position.x, _position.y,
					_position.x - _width, _position.y + _height,
					_position.x - _width, _position.y + _height,
					_position.x, _position.y,
					_position.x, _position.y + _height
				};
				break;
			case ElementAnchor::BottomCentre:
				positions = {
					_position.x - halfwidth, _position.y,
					_position.x + halfwidth, _position.y,
					_position.x - halfwidth, _position.y + _height,
					_position.x - halfwidth, _position.y + _height,
					_position.x + halfwidth, _position.y,
					_position.x + halfwidth, _position.y + _height
				};
				break;
			case ElementAnchor::TopLeft:
				positions = {
					_position.x, _position.y - _height,
					_position.x + _width, _position.y - _height,
					_position.x, _position.y,
					_position.x, _position.y,
					_position.x + _width, _position.y - _height,
					_position.x + _width, _position.y
				};
				break;
			case ElementAnchor::TopRight:
				positions = {
					_position.x - _width, _position.y - _height,
					_position.x, _position.y - _height,
					_position.x - _width, _position.y,
					_position.x - _width, _position.y,
					_position.x, _position.y - _height,
					_position.x, _position.y
				};
				break;
			case ElementAnchor::TopCentre:
				positions = {
					_position.x - halfwidth, _position.y - _height,
					_position.x + halfwidth, _position.y - _height,
					_position.x - halfwidth, _position.y,
					_position.x - halfwidth, _position.y,
					_position.x + halfwidth, _position.y - _height,
					_position.x + halfwidth, _position.y
				};
				break;
			case ElementAnchor::Left:
				positions = {
					_position.x, _position.y - halfheight,
					_position.x + _width, _position.y - halfheight,
					_position.x, _position.y + halfheight,
					_position.x, _position.y + halfheight,
					_position.x + _width, _position.y - halfheight,
					_position.x + _width, _position.y + halfheight
				};
				break;
			case ElementAnchor::Right:
				positions = {
					_position.x - _width, _position.y - halfheight,
					_position.x, _position.y - halfheight,
					_position.x - _width, _position.y + halfheight,
					_position.x - _width, _position.y + halfheight,
					_position.x, _position.y - halfheight,
					_position.x, _position.y + halfheight
				};
				break;
			case ElementAnchor::Centre:
				positions = {
					_position.x - halfwidth, _position.y - halfheight,
					_position.x + halfwidth, _position.y - halfheight,
					_position.x - halfwidth, _position.y + halfheight,
					_position.x - halfwidth, _position.y + halfheight,
					_position.x + halfwidth, _position.y - halfheight,
					_position.x + halfwidth, _position.y + halfheight
				};
				break;
			}
			
			// Destroy the old mesh buffers and data.
			_mesh.Destroy();
			// Create the new mesh with the new data.
			_mesh = ElementMesh { positions, uvs };
		}
		
	}
	
}