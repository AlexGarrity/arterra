#pragma once

#include "PCH.hpp"
#include <glm/glm.hpp>

namespace arterra {
	
	namespace UI {
		
		enum ColliderType {
			Box = 0,
			Circle = 1
		};
		
		class Element;
				
		class Collider {
			
			public:
				Collider();
				Collider(Element* element);
				
				virtual void GenerateCollider() {};
				virtual bool ContainsPoint(glm::vec2 position) { return false; }
				
			protected:
				// Reference to the UI element.
				Element* _element;
				
		};
		
		class BoxCollider : public Collider {
			
			public:
				BoxCollider();
				BoxCollider(Element* element);
				
				virtual void GenerateCollider();
				virtual bool ContainsPoint(glm::vec2 position);
			
			private:
				// List of vertices making up the box.
				// [0] = pos, [1] = width-corner, [2] = height-corner, [3] = opposite pos.
				std::vector<glm::vec2> _vertices;
				
				// Method used to compute barycentric coordinates.
				// Used for collision checking.
				float_t signedDistance(glm::vec2 point, glm::vec2 a, glm::vec2 b);
				// Rotates a point around an origin.
				// Used for rotating the collider around the UI element anchor.
				glm::vec2 rotatePoint(glm::vec2 point, glm::vec2 origin, float_t angle);
				
		};
		
	}
	
}