#pragma once

#include "PCH.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/VertexArray.hpp"
#include "texture/TextureAtlas.hpp"
#include "ui/Collider.hpp"
#include "ui/Material.hpp"

namespace arterra {

	namespace UI {

		/**
		 * @brief The location of the pivot determines the point around
		 * which any transformations happen on a `UI\::Element`.
		 */
		enum class Pivot {
			TopLeft = 0,
			TopCentre = 1,
			TopRight = 2,
			Left = 3,
			Centre = 4,
			Right = 5,
			BottomLeft = 6,
			BottomCentre = 7,
			BottomRight = 8
		};

		// TODO: Implement anchors which scale UI::Element along a length.
		/**
		 * @brief The location of the anchor determines the point/length
		 * around which the `UI\::Element` is positioned/scaled in relation
		 * to its parent.
		 */
		enum class Anchor {
			TopLeft = 0,
			TopCentre = 1,
			TopRight = 2,
			Left = 3,
			Centre = 4,
			Right = 5,
			BottomLeft = 6,
			BottomCentre = 7,
			BottomRight = 8
		};

		struct ElementMesh {

			std::vector<float_t> _posVertices;
			std::vector<float_t> _uvVertices;
			VertexArray _vertexArray;

			VertexBuffer _posBuffer;
			VertexBuffer _texBuffer;

			ElementMesh();
			ElementMesh(std::vector<float_t> posVertices, std::vector<float_t> uvVertices);

			inline void Bind() { _vertexArray.Bind(); }
			inline GLuint GetVertexCount() { return _vertexArray.GetVertexCount(); }

			void GenerateMesh()
			{
				_posBuffer.Create(_posVertices, 2, GL_FLOAT);
				_texBuffer.Create(_uvVertices, 2, GL_FLOAT);
				_vertexArray.AddBuffer(_posBuffer);
				_vertexArray.AddBuffer(_texBuffer);
			}

			void Destroy()
			{
				_vertexArray.Destroy();
				_posBuffer.Destroy();
				_texBuffer.Destroy();
			}
		};

		class Element {

		public:
			Element();
			Element(int width, int height, glm::vec2 position, float_t rotation, Pivot positionAnchor,
				Anchor anchor, AtlasTexture* texture, Material material);

			void ApplyTranslation(glm::vec2 movementVector);
			void ApplyScaling(float_t scaleFactor);
			void ApplyRotation(float_t rotationAngle);

			void Update(glm::vec2 mousePosition, int mouseClick);

			void CreateMesh();
			void CreateCollider();
			void UpdateTransform();

			std::function<void()> OnMouseEnter;
			std::function<void()> OnMouseLeave;
			std::function<void()> OnMouseClick;

			inline glm::vec2 GetPosition() { return _position; }
			inline float_t GetRotation() { return glm::degrees(_rotation); }
			inline Material& GetMaterial() { return _material; }
			inline ElementMesh& GetMesh() { return _mesh; }
			inline Collider* GetCollider() { return _collider; }

			// Returns list of vertices after undoing anchor relativity.
			// TODO: proper documentation
			std::vector<glm::vec2> VerticesFromAnchor(
				Pivot anchor, glm::vec2 bottomLeftPosition, float_t width, float_t height);

			// Width and height of this UI element,
			// in relation to it's pivot point.
			int _width, _height;
			// The location of the pivot point relative to the whole element.
			Pivot _positionAnchor;

		private:
			// The position of the pivot point.
			glm::vec2 _position;
			// The scale applied to the width & height.
			float_t _scale;
			// The rotation about the rotation anchor.
			float_t _rotation;

			// The anchor point responsible for relative positioning.
			Anchor _anchor;

			// Cached state of if mouse is over the UI element.
			bool _isMouseOver;

			// Contains the shader reference and the uniform data for this ui-element.
			Material _material;
			// The mesh data.
			ElementMesh _mesh;
			// The collider data.
			Collider* _collider;
			// The texture handle.
			AtlasTexture* _texture;
			// The "model" transformation matrix.
			glm::mat4 _transform { 1.0f };
		};

	}

}