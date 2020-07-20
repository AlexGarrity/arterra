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
		 * @brief The location of the Pivot determines the point around
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

		// TODO: Implement anchors which scale UI element along a length.
		/**
		 * @brief The location of the Anchor determines the point/length
		 * around which the `UI\::Element` is positioned/scaled in relation
		 * to its `parent`.
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

		/**
		 * @brief Element data structure which holds all the information
		 * regarding a `UI\::Element`. Provides a number of methods for
		 * accessing/modifying data about the element. Provides a static
		 * method `VerticesFromPivot()` which can be used to resolve any
		 * `rectangle/pivot` combo into 4 vertices.
		 */
		class Element {

		public:
			/// ctor
			Element();
			/**
			 * @brief This ctor sets the data values, and the pointers to point to already
			 * existing classes.
			 * @param position The position of the Pivot, relative to the Anchor.
			 * @param width The width of this element.
			 * @param height The height of this element.
			 * @param rotation The initial rotation of this element.
			 * @param pivot The location of the pivot point.
			 * @param anchor The anchor type.
			 * @param parent Pointer to the parent element. Can be NULL.
			 * @param texture Pointer to the texture used for this element.
			 * @param material The material to use for this element.
			 */
			Element(glm::vec2 position, int width, int height, float_t rotation, Pivot pivot, Anchor anchor,
				Element* parent, AtlasTexture* texture, Material material);

			/**
			 * @brief Translate this `UI\::Element` by a vector.
			 * @param movementVector The vector to be added to the
			 * existing position.
			 */
			void ApplyTranslation(glm::vec2 movementVector);
			/**
			 * @brief Scale this `UI\::Element` by a float.
			 * @param scaleDelta The scale difference to be added to
			 * the existing scale factor.
			 */
			void ApplyScaling(float_t scaleDelta);
			/**
			 * @brief Rotate this `UI\::Element` by an angle.
			 * @param rotationAngle The angle (in degrees) to be added
			 * to the existing rotation amount.
			 */
			void ApplyRotation(float_t rotationAngle);

			/**
			 * @brief Runs the mouseEvent logic for this `UI\::Element`.
			 * This method gets called by the `UI\::Manager\::Update()`.
			 * @param mousePosition The position of the mouse, where (0, 0)
			 * is the bottom-left corner.
			 * @param mouseClick The type of mouse button pressed.
			 */
			void Update(glm::vec2 mousePosition, int mouseClick);
			/**
			 * @brief Updates all transform data and the model projection to
			 * be used in the shader.
			 */
			void UpdateTransform();

			/**
			 * @brief Initialises and constructs the mesh data for this
			 * `UI\::Element`.
			 */
			void CreateMesh();
			/**
			 * @brief Initialises and construct the collider for this
			 * `UI\::Element`.
			 */
			void CreateCollider();

			std::function<void()> OnMouseEnter;
			std::function<void()> OnMouseLeave;
			std::function<void()> OnMouseClick;

			/**
			 * @brief Gets the value of the `Pivot` position, in relation to the `Anchor`.
			 * @return Value of `_localPosition`.
			 */
			inline glm::vec2 GetLocalPosition() { return _localPosition; }
			/**
			 * @brief Gets the value of the `Pivot` position, in relation to the whole
			 * `screen`, where bottom-left is (0, 0).
			 * @return Value of `_globalPosition`.
			 */
			inline glm::vec2 GetGlobalPosition() { return _globalPosition; }

			/**
			 * @brief Gets the value of the `width` taking into account the scale factor.
			 * `UI\::Collider` calls this method for generation.
			 * @return Value of the true `width`.
			 */
			inline float_t GetWidth() { return _width * _scale; }
			/**
			 * @brief Gets the value of the `height` taking into account the scale factor.
			 * `UI\::Collider` calls this method for generation.
			 * @return Value of the true `height`
			 */
			inline float_t GetHeight() { return _height * _scale; }
			/**
			 * @brief Gets the value of the `rotation`, in degrees. `UI\::Collider` calls
			 * this method for generation.
			 * @return Value of the `rotation`.
			 */
			inline float_t GetRotation() { return glm::degrees(_rotation); }
			/**
			 * @brief Gets the value of the `scale`.
			 * @return Value of the `scale`.
			 */
			inline float_t GetScale() { return _scale; }
			/**
			 * @brief Gets the value of the `pivot`. `UI\::Collider` calls this method for
			 * generation.
			 * @return Value of the `pivot`.
			 */
			inline Pivot GetPivot() { return _pivot; }
			/**
			 * @brief Gets the value of the `anchor`.
			 * @return Value of the `anchor`.
			 */
			inline Anchor GetAnchor() { return _anchor; }

			/**
			 * @brief Gets a pointer to the `Material`.
			 * @return `Pointer` to the member `_material` stored in this class.
			 */
			inline Material& GetMaterial() { return _material; }
			/**
			 * @brief Gets a pointer to the `Collider`. This can be a `BoxCollider`.
			 * @return `Pointer` to the member `_collider` stored in this class.
			 */
			inline Collider* GetCollider() { return _collider; }
			/**
			 * @brief Gets a pointer to the `ElementMesh`.
			 * @return `Pointer` to the member `_mesh` stored in this class.
			 */
			inline ElementMesh& GetMesh() { return _mesh; }

			/**
			 * @brief Returns 4 vertices which make up a rectangle, based off
			 * the `width`, `height`, and `pivot` of the rectangle.
			 * @param pivot The pivot to undo the transformation from.
			 * @param pivotPosition The position of the pivot.
			 * @param width The width of the rectangle.
			 * @param height The height of the rectangle.
			 * @returns The 4 vertices which make up the rectangle.
			 */
			static std::vector<glm::vec2> VerticesFromPivot(
				Pivot pivot, glm::vec2 pivotPosition, float_t width, float_t height);

		private:
			/// @brief The position of the Pivot, which determines where
			/// the `UI\::Element` appears on screen, in relation to the `Anchor`.
			glm::vec2 _localPosition;
			/// @brief The width/height of the `UI\::Element` at a scale of `1.0`.
			int _width, _height;
			/// @brief The rotation around the Pivot (in radians).
			float_t _rotation;
			/// @brief The scale which is applied to the `width` and `height`.
			float_t _scale;

			/// @brief The location of the pivot point relative to the whole element.
			Pivot _pivot;
			/// @brief The anchor type responsible for relative positioning.
			Anchor _anchor;

			/// @brief `Pointer` to the parent `UI\::Element` of this element.
			Element* _parent;

			/// @brief Cached state of: if mouse is over the `UI\::Element`.
			bool _isMouseOver;
			/// @brief Cached value of: global position relative to screen coordinates.
			glm::vec2 _globalPosition;

			/// @brief The material for this `UI\::element`.
			Material _material;
			/// @brief `Pointer` to the collider, which is inherited from BASE.
			Collider* _collider;
			// The mesh data.
			ElementMesh _mesh;
			// The texture handle.
			AtlasTexture* _texture;
			// The "model" transformation matrix.
			glm::mat4 _transform { 1.0f };

			/**
			 * @brief Converts the `position` & `Anchor` to a position relative
			 * to the parent `UI\::Element`. Stores the cached value to the member
			 * `_globalPosition`.
			 */
			void CalculateGlobalPosition();
		};

	}

}