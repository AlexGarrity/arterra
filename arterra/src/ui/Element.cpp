#include "ui/Element.hpp"

namespace arterra {

	namespace UI {

		ElementMesh::ElementMesh()
			: _posVertices()
			, _uvVertices()
			, _vertexArray()
			, _posBuffer()
			, _texBuffer()
		{
		}

		ElementMesh::ElementMesh(std::vector<float_t> posVertices, std::vector<float_t> uvVertices)
			: _posVertices(posVertices)
			, _uvVertices(uvVertices)
		{
			GenerateMesh();
		}

		Element::Element()
			: _localPosition(glm::vec2(0.0f, 0.0f))
			, _globalPosition(glm::vec2(0.0f, 0.0f))
			, _width(0)
			, _height(0)
			, _rotation(0.0f)
			, _scale(1.0f)
			, _pivot(Pivot::BottomLeft)
			, _anchor(Anchor::BottomLeft)
			, _mesh(ElementMesh())
			, _material(Material())
			, _collider(nullptr)
		{
		}

		Element::Element(glm::vec2 position, int width, int height, float_t rotation, Pivot pivot, Anchor anchor,
			Element* parent, AtlasTexture* texture, Material material)
			: _localPosition(position)
			, _width(width)
			, _height(height)
			, _rotation(glm::radians(rotation))
			, _scale(1.0f)
			, _pivot(pivot)
			, _anchor(anchor)
			, _parent(parent)
			, _material(material)
			, _texture(texture)
			, _collider(nullptr)
		{

			CalculateGlobalPosition();
			_transform = glm::translate(_transform, { _globalPosition.x, _globalPosition.y, 0.0f });
			_transform = glm::rotate(_transform, _rotation, { 0.0f, 0.0f, 1.0f });
			_transform = glm::scale(_transform, { _width, _height, 1.0f });
			_material.AddParameter(
				ShaderParameter { "u_ModelProjection", _transform, ShaderParameter::Type::Mat4 });
			// TODO global position
		}

		void Element::Update(glm::vec2 mousePosition, int mouseClick)
		{
			// Check whether the mouse is clicking over the UI element.
			if (_isMouseOver && mouseClick >= 0) {
				OnMouseClick();
			} else {
				// Check the state of the cursor over the UI element has changed.
				if (GetCollider()->ContainsPoint(mousePosition) && _isMouseOver == false) {
					_isMouseOver = true;
					OnMouseEnter();
				} else if (GetCollider()->ContainsPoint(mousePosition) == false && _isMouseOver) {
					_isMouseOver = false;
					OnMouseLeave();
				}
			}
		}

		void Element::UpdateTransform()
		{
			// NOTE WHEN TRANSFORMING!!!
			// Translate -> Rotate -> Scale ALWAYS!!!
			// Also, translate is not commutative,
			//		i.e, you can't add movement vector to existing position.
			CalculateGlobalPosition();
			_transform = glm::mat4 { 1.0f };
			_transform = glm::translate(_transform, { _globalPosition.x, _globalPosition.y, 0.0f });
			_transform = glm::rotate(_transform, _rotation, { 0.0f, 0.0f, -1.0f });
			_transform = glm::scale(_transform, { GetWidth(), GetHeight(), 1.0f });
			_material.UpdateParameter("u_ModelProjection", _transform);
			// Regenerate the collider data based off the new transform.
			_collider->GenerateCollider();
		}

		void Element::ApplyTranslation(glm::vec2 movementVector)
		{
			_localPosition.x += movementVector.x;
			_localPosition.y += movementVector.y;
		}

		void Element::ApplyRotation(float_t rotationAngle)
		{
			rotationAngle = glm::radians(rotationAngle);
			// If rotation is greater than 360 degrees, loop back round to 0.
			if (rotationAngle > glm::pi<float>()) {
				rotationAngle = rotationAngle - glm::pi<float>();
			}
			_rotation += rotationAngle;
		}

		void Element::ApplyScaling(float_t scaleDelta) { _scale += scaleDelta; }

		void Element::CalculateGlobalPosition()
		{
			glm::vec2 pos { 0.0f, 0.0f };
			// TODO: Read these values from the actual window dimensions.
			float_t width = 1280.0f;
			float_t height = 720.0f;
			float_t halfWidth = width / 2;
			float_t halfHeight = height / 2;

			if (_parent == nullptr) {
				// This UI element has no parent, so the Anchor is relative
				// to the screen dimensions.

				switch (_anchor) {
					case Anchor::BottomLeft:
						pos = _localPosition;
						break;
					case Anchor::BottomCentre:
						pos = glm::vec2(_localPosition.x + halfWidth, _localPosition.y);
						break;
					case Anchor::BottomRight:
						pos = glm::vec2(_localPosition.x + width, _localPosition.y);
						break;
					case Anchor::TopLeft:
						pos = glm::vec2(_localPosition.x, _localPosition.y + height);
						break;
					case Anchor::TopCentre:
						pos = glm::vec2(_localPosition.x + halfWidth, _localPosition.y + height);
						break;
					case Anchor::TopRight:
						pos = glm::vec2(_localPosition.x + width, _localPosition.y + height);
						break;
					case Anchor::Left:
						pos = glm::vec2(_localPosition.x, _localPosition.y + halfHeight);
						break;
					case Anchor::Centre:
						pos = glm::vec2(_localPosition.x + halfWidth, _localPosition.y + halfHeight);
						break;
					case Anchor::Right:
						pos = glm::vec2(_localPosition.x + width, _localPosition.y + halfHeight);
						break;
				}
			}
			_globalPosition = pos;
		}

		std::vector<glm::vec2> Element::VerticesFromPivot(
			Pivot pivot, glm::vec2 pivotPosition, float_t width, float_t height)
		{

			float_t halfWidth = width / 2;
			float_t halfHeight = height / 2;

			// Create and reserve room for 4 vertices.
			std::vector<glm::vec2> vertices;
			vertices.reserve(4);

			switch (pivot) {
				case Pivot::BottomLeft:
					vertices = { glm::vec2(pivotPosition.x, pivotPosition.y),
						glm::vec2(pivotPosition.x + width, pivotPosition.y),
						glm::vec2(pivotPosition.x, pivotPosition.y + height),
						glm::vec2(pivotPosition.x + width, pivotPosition.y + height) };
					break;
				case Pivot::BottomCentre:
					vertices = { glm::vec2(pivotPosition.x - halfWidth, pivotPosition.y),
						glm::vec2(pivotPosition.x + halfWidth, pivotPosition.y),
						glm::vec2(pivotPosition.x - halfWidth, pivotPosition.y + height),
						glm::vec2(pivotPosition.x + halfWidth, pivotPosition.y + height) };
					break;
				case Pivot::BottomRight:
					vertices = { glm::vec2(pivotPosition.x - width, pivotPosition.y),
						glm::vec2(pivotPosition.x, pivotPosition.y),
						glm::vec2(pivotPosition.x - width, pivotPosition.y + height),
						glm::vec2(pivotPosition.x, pivotPosition.y + height) };
					break;
				case Pivot::TopLeft:
					vertices = { glm::vec2(pivotPosition.x, pivotPosition.y - height),
						glm::vec2(pivotPosition.x + width, pivotPosition.y - height),
						glm::vec2(pivotPosition.x, pivotPosition.y),
						glm::vec2(pivotPosition.x + width, pivotPosition.y) };
					break;
				case Pivot::TopCentre:
					vertices = { glm::vec2(pivotPosition.x - halfWidth, pivotPosition.y - height),
						glm::vec2(pivotPosition.x + halfWidth, pivotPosition.y - height),
						glm::vec2(pivotPosition.x - halfWidth, pivotPosition.y),
						glm::vec2(pivotPosition.x + halfWidth, pivotPosition.y) };
					break;
				case Pivot::TopRight:
					vertices = { glm::vec2(pivotPosition.x - width, pivotPosition.y - height),
						glm::vec2(pivotPosition.x, pivotPosition.y - height),
						glm::vec2(pivotPosition.x - width, pivotPosition.y),
						glm::vec2(pivotPosition.x, pivotPosition.y) };
					break;
				case Pivot::Left:
					vertices = { glm::vec2(pivotPosition.x, pivotPosition.y - halfHeight),
						glm::vec2(pivotPosition.x + width, pivotPosition.y - halfHeight),
						glm::vec2(pivotPosition.x, pivotPosition.y + halfHeight),
						glm::vec2(pivotPosition.x + width, pivotPosition.y + halfHeight) };
					break;
				case Pivot::Right:
					vertices = { glm::vec2(pivotPosition.x - width, pivotPosition.y - halfHeight),
						glm::vec2(pivotPosition.x, pivotPosition.y - halfHeight),
						glm::vec2(pivotPosition.x - width, pivotPosition.y + halfHeight),
						glm::vec2(pivotPosition.x, pivotPosition.y + halfHeight) };
					break;
				case Pivot::Centre:
					vertices = { glm::vec2(pivotPosition.x - halfWidth, pivotPosition.y - halfHeight),
						glm::vec2(pivotPosition.x + halfWidth, pivotPosition.y - halfHeight),
						glm::vec2(pivotPosition.x - halfWidth, pivotPosition.y + halfHeight),
						glm::vec2(pivotPosition.x + halfWidth, pivotPosition.y + halfHeight) };
					break;
			}
			return vertices;
		}

		void Element::CreateCollider()
		{
			// TODO: Differentiate between collider types on creation.
			if (_collider) {
				delete _collider;
			}
			_collider = new BoxCollider(this);
		}

		void Element::CreateMesh()
		{
			// Reserve space for vertex position data.
			std::vector<float_t> positions;
			positions.resize(12);

			// Translate uv 0-1 coordinates to atlas coordinates.
			std::vector<float_t> uvs = { _texture->_x, _texture->_y, _texture->_x + _texture->_width, _texture->_y,
				_texture->_x, _texture->_y + _texture->_height, _texture->_x, _texture->_y + _texture->_height,
				_texture->_x + _texture->_width, _texture->_y, _texture->_x + _texture->_width,
				_texture->_y + _texture->_height };

			float_t halfwidth = 0.5f;
			float_t halfheight = 0.5f;

			// Calculate the vertex positions based off the pivot point.
			switch (_pivot) {
				case Pivot::BottomLeft:
					positions = { 0.0f, 0.0f, 0.0f + 1.0f, 0.0f, 0.0f, 0.0f + 1.0f, 0.0f, 0.0f + 1.0f, 0.0f + 1.0f,
						0.0f, 0.0f + 1.0f, 0.0f + 1.0f };
					break;
				case Pivot::BottomRight:
					positions = { 0.0f - 1.0f, 0.0f, 0.0f, 0.0f, 0.0f - 1.0f, 0.0f + 1.0f, 0.0f - 1.0f,
						0.0f + 1.0f, 0.0f, 0.0f, 0.0f, 0.0f + 1.0f };
					break;
				case Pivot::BottomCentre:
					positions = { 0.0f - halfwidth, 0.0f, 0.0f + halfwidth, 0.0f, 0.0f - halfwidth, 0.0f + 1.0f,
						0.0f - halfwidth, 0.0f + 1.0f, 0.0f + halfwidth, 0.0f, 0.0f + halfwidth, 0.0f + 1.0f };
					break;
				case Pivot::TopLeft:
					positions = { 0.0f, 0.0f - 1.0f, 0.0f + 1.0f, 0.0f - 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f + 1.0f,
						0.0f - 1.0f, 0.0f + 1.0f, 0.0f };
					break;
				case Pivot::TopRight:
					positions = { 0.0f - 1.0f, 0.0f - 1.0f, 0.0f, 0.0f - 1.0f, 0.0f - 1.0f, 0.0f, 0.0f - 1.0f,
						0.0f, 0.0f, 0.0f - 1.0f, 0.0f, 0.0f };
					break;
				case Pivot::TopCentre:
					positions = { 0.0f - halfwidth, 0.0f - 1.0f, 0.0f + halfwidth, 0.0f - 1.0f, 0.0f - halfwidth,
						0.0f, 0.0f - halfwidth, 0.0f, 0.0f + halfwidth, 0.0f - 1.0f, 0.0f + halfwidth, 0.0f };
					break;
				case Pivot::Left:
					positions = { 0.0f, 0.0f - halfheight, 0.0f + 1.0f, 0.0f - halfheight, 0.0f, 0.0f + halfheight,
						0.0f, 0.0f + halfheight, 0.0f + 1.0f, 0.0f - halfheight, 0.0f + 1.0f, 0.0f + halfheight };
					break;
				case Pivot::Right:
					positions = { 0.0f - 1.0f, 0.0f - halfheight, 0.0f, 0.0f - halfheight, 0.0f - 1.0f,
						0.0f + halfheight, 0.0f - 1.0f, 0.0f + halfheight, 0.0f, 0.0f - halfheight, 0.0f,
						0.0f + halfheight };
					break;
				case Pivot::Centre:
					positions = { 0.0f - halfwidth, 0.0f - halfheight, 0.0f + halfwidth, 0.0f - halfheight,
						0.0f - halfwidth, 0.0f + halfheight, 0.0f - halfwidth, 0.0f + halfheight, 0.0f + halfwidth,
						0.0f - halfheight, 0.0f + halfwidth, 0.0f + halfheight };
					break;
			}

			// Create the mesh with the new data.
			_mesh = ElementMesh { positions, uvs };
		}

	}

}