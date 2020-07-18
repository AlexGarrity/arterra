#include "ui/Collider.hpp"
#include "ui/Element.hpp"

namespace arterra {
	
	namespace UI {
		
		ElementCollider::ElementCollider()
			: _element(nullptr), _vertices() {}
		
		ElementCollider::ElementCollider(Element* element)
			: _element(element), _vertices() {
			// Reserve the space needed for the vertices.
			_vertices.reserve(4);
			GenerateCollider();
		}
			
		void ElementCollider::GenerateCollider() {
			// Avoid having to fetch the data many times from the element.
			float_t rotation = _element->GetRotation();
			glm::vec2 position = _element->GetPosition();
			float_t width = _element->_width;
			float_t height = _element->_height;
			
			// Clear any previous vertex data.
			_vertices.clear();
			_simpleCheck = true;
			
			// For 90-degree rotation intervals, the vertex positions
			// are predefined to correct values. This is quicker than performing
			// a "proper" rotation using trigonometry.
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
				// The rotation does not lie of 90-degree intervals.
				// In this case, a proper rotation must be done on all vertices.
				// This also invalidates the simple check, as the more
				// advanced method must be used in 'IsInside()'.
				_simpleCheck = false;
				
				// TODO: currently assuming rotation anchor is bottom-left,
				// hence _vertices[0] doesn't get rotated as it's the anchor.
				_vertices.push_back(glm::vec2(position.x, position.y));
				_vertices.push_back(rotatePoint(glm::vec2(position.x + width, position.y),
					glm::vec2(position.x, position.y), glm::radians(rotation)));
				_vertices.push_back(rotatePoint(glm::vec2(position.x, position.y + height),
					glm::vec2(position.x, position.y), glm::radians(rotation)));
				_vertices.push_back(rotatePoint(glm::vec2(position.x + width, position.y + height),
					glm::vec2(position.x, position.y), glm::radians(rotation)));
			}
			
		}
		
		glm::vec2 ElementCollider::rotatePoint(glm::vec2 point, glm::vec2 origin, float_t angle) {
			// Translate point to be relative to origin, i.e. origin is (0,0).
			point -= origin;
			// Apply the rotation.
			glm::vec2 result {
				(point.x * glm::cos(-angle)) - (point.y * glm::sin(-angle)),
				(point.x * glm::sin(-angle)) + (point.y * glm::cos(-angle))
			};
			// Undo the translation.
			return result + origin;
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
			
			// Test using the Barycentric coordinate method.
			// Explanation: https://youtu.be/HYAgJN3x4GA
			float_t d1, d2, d3;
			bool neg, pos;
			bool result1, result2;
			glm::vec2 point = position;
			
			// Test first triangle 0,1,2.
			// vertices[0] = A, vertices[1] = B, vertices[2] = C, point = P
			d1 = signedDistance(point, _vertices[0], _vertices[1]);
			d2 = signedDistance(point, _vertices[1], _vertices[2]);
			d3 = signedDistance(point, _vertices[2], _vertices[0]);
			
			// Check if the point is definitely inside the triangle (all distances are negative)
			neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
			// Check if the point is definitely outside the triangle (all distances are positive)
			pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
			
			// Whether the point is inside or outside the triangle.
			// If inside, 'neg' will be true and 'pos' will be false.
			// If outside, 'neg' and 'pos' will be true.
			// Result inverted to make logical sense, otherwise true<->false
			// would be flipped.
			result1 = !(neg && pos);
			
			// Test second triangle 2,1,3.
			// vertices[2] = A, vertices[1] = B, vertices[3] = C, point = P
			d1 = signedDistance(point, _vertices[2], _vertices[1]);
			d2 = signedDistance(point, _vertices[1], _vertices[3]);
			d3 = signedDistance(point, _vertices[3], _vertices[2]);
			neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
			pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
			result2 = !(neg && pos);
			
			// If either of the triangles are colliding,
			// return success.
			return (result1 || result2) ? true : false;
			
		}
		
		float_t ElementCollider::signedDistance(glm::vec2 point, glm::vec2 a, glm::vec2 b) {
			// Compute the cross product of the point and the two triangle points which make a line
			// for it to intercept.
			// This essentially computes the inverse gradient of the line, then checks
			// which side of the line the given point is on.
			// This basically computes how far along A-B line you must move (optionally before
			// changing direction) to get to Point.
			return (point.x - b.x) * (a.y - b.y) - (a.x - b.x) * (point.y - b.y);
		}
		
	}
	
}