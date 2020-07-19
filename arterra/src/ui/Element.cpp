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
			: _width(0), _height(0), _position(glm::vec2(0.0f, 0.0f)), _rotation(0.0f),
			_positionAnchor(Pivot::BottomLeft),	_mesh(ElementMesh()), _material(Material()),
			_collider(nullptr) {}
		
		Element::Element(int width, int height, glm::vec2 position, float_t rotation, Pivot positionAnchor,
			Anchor anchor, AtlasTexture* texture, Material material)
			: _width(width), _height(height), _position(position), _rotation(glm::radians(rotation)),
			_positionAnchor(positionAnchor), _anchor(anchor), _material(material), _texture(texture),
			_collider(nullptr) {
			
			_transform = glm::translate(_transform, { position.x, position.y, 0.0f });
			_transform = glm::scale(_transform, { width , height, 1.0f });
			_transform = glm::rotate(_transform, 0.0f, { 0.0f, 0.0f, 1.0f });
			_material.AddParameter(ShaderParameter { "u_ModelProjection", _transform, ShaderParameter::Type::Mat4});
			
		}
		
		void Element::Update(glm::vec2 mousePosition, int mouseClick) {
			// Check whether the mouse is clicking over the UI element.
			if (_isMouseOver && mouseClick >= 0) {
				std::cout << "CLICKED" << std::endl;
			}else {
				// Check the state of the cursor over the UI element has changed.
				if (GetCollider()->ContainsPoint(mousePosition) && _isMouseOver == false) {
					_isMouseOver = true;
					OnMouseEnter();
				}else if (GetCollider()->ContainsPoint(mousePosition) == false && _isMouseOver) {
					_isMouseOver = false;
					OnMouseLeave();
				}
			}
			
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
			rotationAngle = glm::radians(rotationAngle);
			if (rotationAngle > glm::pi<float>()) {
				rotationAngle = rotationAngle - glm::pi<float>();
			}
			_rotation += rotationAngle;
		}
		
		void Element::UpdateTransform() {
			// NOTE WHEN TRANSFORMING!!!
			// Translate -> Rotate -> Scale ALWAYS!!!
			// Also, translate is not commutative,
			//		i.e, you can't add movement vector to existing position.
			_transform = glm::mat4 { 1.0f };
			_transform = glm::translate(_transform, { _position.x, _position.y, 0.0f });
			_transform = glm::rotate(_transform, _rotation, { 0.0f, 0.0f, -1.0f });
			_transform = glm::scale(_transform, { _width , _height, 1.0f });
			_material.UpdateParameter("u_ModelProjection", _transform);
			_collider->GenerateCollider();
		}
		
		void Element::CreateCollider() {
			// TODO: differentiate collider types on creation.
			if (_collider) {
				delete _collider;
			}
			_collider = new BoxCollider(this);
		}
		
		std::vector<glm::vec2> Element::VerticesFromAnchor(Pivot anchor, glm::vec2 anchorPosition,
			float_t width, float_t height) {
			
			float_t halfWidth = width / 2;
			float_t halfHeight = height / 2;
			
			std::vector<glm::vec2> vertices;
			vertices.reserve(4);
			
			switch (anchor)
			{
			case Pivot::BottomLeft:
				vertices = {
					glm::vec2(anchorPosition.x, anchorPosition.y),
					glm::vec2(anchorPosition.x + width, anchorPosition.y),
					glm::vec2(anchorPosition.x, anchorPosition.y + height),
					glm::vec2(anchorPosition.x + width, anchorPosition.y + height)
				};
				break;
			case Pivot::BottomCentre:
				vertices = {
					glm::vec2(anchorPosition.x - halfWidth, anchorPosition.y),
					glm::vec2(anchorPosition.x + halfWidth, anchorPosition.y),
					glm::vec2(anchorPosition.x - halfWidth, anchorPosition.y + height),
					glm::vec2(anchorPosition.x + halfWidth, anchorPosition.y + height)
				};
				break;
			case Pivot::BottomRight:
				vertices = {
					glm::vec2(anchorPosition.x - width, anchorPosition.y),
					glm::vec2(anchorPosition.x, anchorPosition.y),
					glm::vec2(anchorPosition.x - width, anchorPosition.y + height),
					glm::vec2(anchorPosition.x, anchorPosition.y + height)
				};
				break;
			case Pivot::TopLeft:
				vertices = {
					glm::vec2(anchorPosition.x, anchorPosition.y - height),
					glm::vec2(anchorPosition.x + width, anchorPosition.y - height),
					glm::vec2(anchorPosition.x, anchorPosition.y),
					glm::vec2(anchorPosition.x + width, anchorPosition.y)
				};
				break;
			case Pivot::TopCentre:
				vertices = {
					glm::vec2(anchorPosition.x - halfWidth, anchorPosition.y - height),
					glm::vec2(anchorPosition.x + halfWidth, anchorPosition.y - height),
					glm::vec2(anchorPosition.x - halfWidth, anchorPosition.y),
					glm::vec2(anchorPosition.x + halfWidth, anchorPosition.y)
				};
				break;
			case Pivot::TopRight:
				vertices = {
					glm::vec2(anchorPosition.x - width, anchorPosition.y - height),
					glm::vec2(anchorPosition.x, anchorPosition.y - height),
					glm::vec2(anchorPosition.x - width, anchorPosition.y),
					glm::vec2(anchorPosition.x, anchorPosition.y)
				};
				break;
			case Pivot::Left:
				vertices = {
					glm::vec2(anchorPosition.x, anchorPosition.y - halfHeight),
					glm::vec2(anchorPosition.x + width, anchorPosition.y - halfHeight),
					glm::vec2(anchorPosition.x, anchorPosition.y + halfHeight),
					glm::vec2(anchorPosition.x + width, anchorPosition.y + halfHeight)
				};
				break;
			case Pivot::Right:
				vertices = {
					glm::vec2(anchorPosition.x - width, anchorPosition.y - halfHeight),
					glm::vec2(anchorPosition.x, anchorPosition.y - halfHeight),
					glm::vec2(anchorPosition.x - width, anchorPosition.y + halfHeight),
					glm::vec2(anchorPosition.x, anchorPosition.y + halfHeight)
				};
				break;
			case Pivot::Centre:
				vertices = {
					glm::vec2(anchorPosition.x - halfWidth, anchorPosition.y - halfHeight),
					glm::vec2(anchorPosition.x + halfWidth, anchorPosition.y - halfHeight),
					glm::vec2(anchorPosition.x - halfWidth, anchorPosition.y + halfHeight),
					glm::vec2(anchorPosition.x + halfWidth, anchorPosition.y + halfHeight)
				};
				break;
			}
			return vertices;
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
			
			float_t halfwidth = 0.5f;
			float_t halfheight = 0.5f;
			
			// Calculate the vertex positions based off the anchor point.
			switch (_positionAnchor)
			{
			case Pivot::BottomLeft:
				positions = {
					0.0f, 0.0f,
					0.0f + 1.0f, 0.0f,
					0.0f, 0.0f + 1.0f,
					0.0f, 0.0f + 1.0f,
					0.0f + 1.0f, 0.0f,
					0.0f + 1.0f, 0.0f + 1.0f
				};
				break;
			case Pivot::BottomRight:
				positions = {
					0.0f - 1.0f, 0.0f,
					0.0f, 0.0f,
					0.0f - 1.0f, 0.0f + 1.0f,
					0.0f - 1.0f, 0.0f + 1.0f,
					0.0f, 0.0f,
					0.0f, 0.0f + 1.0f
				};
				break;
			case Pivot::BottomCentre:
				positions = {
					0.0f - halfwidth, 0.0f,
					0.0f + halfwidth, 0.0f,
					0.0f - halfwidth, 0.0f + 1.0f,
					0.0f - halfwidth, 0.0f + 1.0f,
					0.0f + halfwidth, 0.0f,
					0.0f + halfwidth, 0.0f + 1.0f
				};
				break;
			case Pivot::TopLeft:
				positions = {
					0.0f, 0.0f - 1.0f,
					0.0f + 1.0f, 0.0f - 1.0f,
					0.0f, 0.0f,
					0.0f, 0.0f,
					0.0f + 1.0f, 0.0f - 1.0f,
					0.0f + 1.0f, 0.0f
				};
				break;
			case Pivot::TopRight:
				positions = {
					0.0f - 1.0f, 0.0f - 1.0f,
					0.0f, 0.0f - 1.0f,
					0.0f - 1.0f, 0.0f,
					0.0f - 1.0f, 0.0f,
					0.0f, 0.0f - 1.0f,
					0.0f, 0.0f
				};
				break;
			case Pivot::TopCentre:
				positions = {
					0.0f - halfwidth, 0.0f - 1.0f,
					0.0f + halfwidth, 0.0f - 1.0f,
					0.0f - halfwidth, 0.0f,
					0.0f - halfwidth, 0.0f,
					0.0f + halfwidth, 0.0f - 1.0f,
					0.0f + halfwidth, 0.0f
				};
				break;
			case Pivot::Left:
				positions = {
					0.0f, 0.0f - halfheight,
					0.0f + 1.0f, 0.0f - halfheight,
					0.0f, 0.0f + halfheight,
					0.0f, 0.0f + halfheight,
					0.0f + 1.0f, 0.0f - halfheight,
					0.0f + 1.0f, 0.0f + halfheight
				};
				break;
			case Pivot::Right:
				positions = {
					0.0f - 1.0f, 0.0f - halfheight,
					0.0f, 0.0f - halfheight,
					0.0f - 1.0f, 0.0f + halfheight,
					0.0f - 1.0f, 0.0f + halfheight,
					0.0f, 0.0f - halfheight,
					0.0f, 0.0f + halfheight
				};
				break;
			case Pivot::Centre:
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