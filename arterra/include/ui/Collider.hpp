#pragma once

#include "PCH.hpp"
#include <glm/glm.hpp>

namespace arterra {
	
	namespace UI {
		
		class Element;
				
		class ElementCollider {
			
			public:
				ElementCollider();
				ElementCollider(Element* element);
				
				void GenerateCollider();
				bool IsInside(glm::vec2 position);
				bool _simpleCheck;
				
			private:
				// Reference to parent ui-element.
				Element* _element;
				// [0] = pos, [1] = width-corner, [2] = height-corner, [3] = opposite pos
				std::vector<glm::vec2> _vertices;
				
				float_t signedDistance(glm::vec2 point, glm::vec2 a, glm::vec2 b);
				glm::vec2 rotatePoint(glm::vec2 point, glm::vec2 origin, float_t angle);
				
		};
		
	}
	
}