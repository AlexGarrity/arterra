#include "ui/Element.hpp"

namespace arterra {
	
	namespace UI {
		
		ElementMesh::ElementMesh()
			: _posVertices(), _uvVertices(), _vertexArray(), _posBuffer(), _texBuffer() {}
		
		ElementMesh::ElementMesh(std::vector<float_t> posVertices, std::vector<float_t> uvVertices)
			: _posVertices(posVertices), _uvVertices(uvVertices) {
			GenerateMesh();
		}
		
		ElementCollider::ElementCollider()
			: _element(nullptr), _vertices() {}
		
		ElementCollider::ElementCollider(Element* element)
			: _element(element), _vertices() {
			GenerateCollider();
		}
			
		void ElementCollider::GenerateCollider() {
			float_t rotation = _element->GetRotation();
			std::cout << "ROT " << rotation << std::endl;
			glm::vec2 position = _element->GetPosition();
			float_t width = _element->_width;
			float_t height = _element->_height;
			
			// First clear any previous collider data.
			_vertices.clear();
			
			// Set up the vertices vector.
			_vertices.reserve(4);
			_simpleCheck = true;
			if (rotation == 0.0f) {
				_vertices = {
					glm::vec2(position.x, position.y),
					glm::vec2(position.x + width, position.y),
					glm::vec2(position.x, position.y + height),
					glm::vec2(position.x + width, position.y + height)
				};
			}else if (rotation == 90.0f) {
				_vertices = {
					glm::vec2(position.x, position.y - width),
					glm::vec2(position.x + height, position.y - width),
					glm::vec2(position.x, position.y),
					glm::vec2(position.x + height, position.y)
				};
			}else if (rotation == 180.0f) {
				_vertices = {
					glm::vec2(position.x - width, position.y - height),
					glm::vec2(position.x, position.y - height),
					glm::vec2(position.x - width, position.y),
					glm::vec2(position.x, position.y)
				};
			}else if (rotation == 270.0f) {
				_vertices = {
					glm::vec2(position.x - height),
					glm::vec2(position.x, position.y),
					glm::vec2(position.x - height, position.y + width),
					glm::vec2(position.x, position.y + height)
				};
			}else {
				_simpleCheck = false;
			}
			
			
		}
		
		bool ElementCollider::IsInside(glm::vec2 position) {
			
			// Do a less computationally expensive checking method if
			// the element is rotated on 90-degree intervals.
			if (_simpleCheck) {
				
				if ((position.x > _vertices[0].x) &&
					(position.x < _vertices[3].x) &&
					(position.y > _vertices[0].y) &&
					(position.y < _vertices[3].y)) {
					return true;
				}else {
					return false;
				}
			}
			return false;
		}
		
		Element::Element() 
			: _width(0), _height(0), _position(glm::vec2(0.0f, 0.0f)), _rotation(0.0f),
			_anchor(ElementAnchor::BottomLeft),	_mesh(ElementMesh()), _material(Material()),
			_collider(ElementCollider()) {}
		
		Element::Element(int width, int height, glm::vec2 position, float_t rotation, ElementAnchor anchor,
			AtlasTexture* texture, Material material)
			: _width(width), _height(height), _position(position), _rotation(glm::radians(rotation)), _anchor(anchor),
			_material(material), _texture(texture) {
			
			_transform = glm::translate(_transform, { position.x, position.y, 0.0f });
			_transform = glm::scale(_transform, { width , height, 1.0f });
			_transform = glm::rotate(_transform, 0.0f, { 0.0f, 0.0f, 1.0f });
			_material.AddParameter(ShaderParameter { "u_ModelProjection", _transform, ShaderParameter::Type::Mat4});
			_material.AddParameter(ShaderParameter { "u_Rotation", _rotation, ShaderParameter::Type::Float});
		}
				
		void Element::ApplyTranslation(glm::vec2 movementVector) {
			_position.x += movementVector.x;
			_position.y += movementVector.y;
		}
		
		void Element::ApplyScaling(float_t scaleFactor) {
			_width += scaleFactor;
			_height += scaleFactor;
		}
		
		void Element::ApplyRotation(float_t rotationAngle) {
			// TODO: if greater than 360 loop round to 0.
			rotationAngle = glm::radians(rotationAngle);
			_rotation += rotationAngle;
		}
		
		void Element::UpdateTransform() {
			// WHEN TRANSFORMING!!!
			// Translate -> Rotate -> Scale ALWAYS!!!
			// Also, translate is not commutative,
			//		i.e, you can't add movement vector to existing position.
			_transform = glm::mat4 { 1.0f };
			_transform = glm::translate(_transform, { _position.x, _position.y, 0.0f });
			_transform = glm::rotate(_transform, _rotation, { 0.0f, 0.0f, -1.0f });
			_transform = glm::scale(_transform, { _width , _height, 1.0f });
			_material.UpdateParameter("u_ModelProjection", _transform);
			_collider.GenerateCollider();
		}
		
		void Element::CreateCollider() {
			_collider = ElementCollider { this };
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
			
			int halfwidth = 1 / 2;
			int halfheight = 1 / 2;
			
			// Calculate the vertex positions based off the anchor point.
			switch (_anchor)
			{
			case ElementAnchor::BottomLeft:
				positions = {
					0.0f, 0.0f,
					0.0f + 1.0f, 0.0f,
					0.0f, 0.0f + 1.0f,
					0.0f, 0.0f + 1.0f,
					0.0f + 1.0f, 0.0f,
					0.0f + 1.0f, 0.0f + 1.0f
				};
				break;
			case ElementAnchor::BottomRight:
				positions = {
					0.0f - 1.0f, 0.0f,
					0.0f, 0.0f,
					0.0f - 1.0f, 0.0f + 1.0f,
					0.0f - 1.0f, 0.0f + 1.0f,
					0.0f, 0.0f,
					0.0f, 0.0f + 1.0f
				};
				break;
			case ElementAnchor::BottomCentre:
				positions = {
					0.0f - halfwidth, 0.0f,
					0.0f + halfwidth, 0.0f,
					0.0f - halfwidth, 0.0f + 1.0f,
					0.0f - halfwidth, 0.0f + 1.0f,
					0.0f + halfwidth, 0.0f,
					0.0f + halfwidth, 0.0f + 1.0f
				};
				break;
			case ElementAnchor::TopLeft:
				positions = {
					0.0f, 0.0f - 1.0f,
					0.0f + 1.0f, 0.0f - 1.0f,
					0.0f, 0.0f,
					0.0f, 0.0f,
					0.0f + 1.0f, 0.0f - 1.0f,
					0.0f + 1.0f, 0.0f
				};
				break;
			case ElementAnchor::TopRight:
				positions = {
					0.0f - 1.0f, 0.0f - 1.0f,
					0.0f, 0.0f - 1.0f,
					0.0f - 1.0f, 0.0f,
					0.0f - 1.0f, 0.0f,
					0.0f, 0.0f - 1.0f,
					0.0f, 0.0f
				};
				break;
			case ElementAnchor::TopCentre:
				positions = {
					0.0f - halfwidth, 0.0f - 1.0f,
					0.0f + halfwidth, 0.0f - 1.0f,
					0.0f - halfwidth, 0.0f,
					0.0f - halfwidth, 0.0f,
					0.0f + halfwidth, 0.0f - 1.0f,
					0.0f + halfwidth, 0.0f
				};
				break;
			case ElementAnchor::Left:
				positions = {
					0.0f, 0.0f - halfheight,
					0.0f + 1.0f, 0.0f - halfheight,
					0.0f, 0.0f + halfheight,
					0.0f, 0.0f + halfheight,
					0.0f + 1.0f, 0.0f - halfheight,
					0.0f + 1.0f, 0.0f + halfheight
				};
				break;
			case ElementAnchor::Right:
				positions = {
					0.0f - 1.0f, 0.0f - halfheight,
					0.0f, 0.0f - halfheight,
					0.0f - 1.0f, 0.0f + halfheight,
					0.0f - 1.0f, 0.0f + halfheight,
					0.0f, 0.0f - halfheight,
					0.0f, 0.0f + halfheight
				};
				break;
			case ElementAnchor::Centre:
				positions = {
					0.0f - halfwidth, 0.0f - halfheight,
					0.0f + halfwidth, 0.0f - halfheight,
					0.0f - halfwidth, 0.0f + halfheight,
					0.0f - halfwidth, 0.0f + halfheight,
					0.0f + halfwidth, 0.0f - halfheight,
					0.0f + halfwidth, 0.0f + halfheight
				};
				break;
			}
			/* switch (_anchor)
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
			} */
			
			// Create the mesh with the new data.
			_mesh = ElementMesh { positions, uvs };
		}
		
	}
	
}