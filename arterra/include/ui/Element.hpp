#pragma once

#include "PCH.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/VertexArray.hpp"
#include "ui/Material.hpp"
#include "texture/TextureAtlas.hpp"

namespace arterra {
	
	namespace UI {
		
		enum ElementAnchor {
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
			
			void GenerateMesh() {
				_posBuffer.Create(_posVertices, 2, GL_FLOAT);
				_texBuffer.Create(_uvVertices, 2, GL_FLOAT);
				_vertexArray.AddBuffer(_posBuffer);
				_vertexArray.AddBuffer(_texBuffer);
			}
			
			void Destroy() {
				_vertexArray.Destroy();
				_posBuffer.Destroy();
				_texBuffer.Destroy();
			}
			
		};
		
		struct ElementCollider {
			
			ElementCollider();
			
			void GenerateCollider();
			bool IsInside(glm::vec2 position);
			
		};
		
		class Element {
			
			public:
				Element();
				Element(int width, int height, glm::vec2 position, float_t rotation, ElementAnchor anchor,
					AtlasTexture* texture, Material material);
				
				void ApplyTranslation(glm::vec2 movementVector);
				void ApplyScaling(float_t scaleFactor);
				void ApplyRotation(float_t rotationAngle);
				
				void CreateMesh();
				void UpdateTransform();
				
				inline glm::vec2 GetPosition() { return _position; }
				inline Material& GetMaterial() { return _material; }
				inline ElementMesh& GetMesh() { return _mesh; }
				
			private:
				// Width and height of this ui-element.
				int _width, _height;
				// The position of the anchor point.
				glm::vec2 _position;
				// The rotation about the anchor point.
				float_t _rotation;
				// The position of the anchor.
				ElementAnchor _anchor;
				
				// Contains the shader reference and the uniform data for this ui-element.
				Material _material;
				// The mesh data.
				ElementMesh _mesh;
				// The texture handle.
				AtlasTexture* _texture;
				// The "model" transformation matrix.
				glm::mat4 _transform { 1.0f };
				
		};
		
	}
	
}