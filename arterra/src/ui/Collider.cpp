#include "ui/Collider.hpp"
#include "ui/Element.hpp"

namespace arterra {

	namespace UI {

		Collider::Collider()
			: _element(nullptr)
		{
		}

		Collider::Collider(Element* element)
			: _element(element)
		{
		}

		BoxCollider::BoxCollider()
			: Collider()
			, _vertices()
		{
		}

		BoxCollider::BoxCollider(Element* element)
			: Collider(element)
			, _vertices()
		{
			// Reserve the space needed for the box vertices.
			_vertices.reserve(4);
			GenerateCollider();
		}

		void BoxCollider::GenerateCollider()
		{
			// Avoid having to fetch the data many times from the element.
			glm::vec2 origin = _element->GetGlobalPosition();
			float_t width = _element->GetWidth();
			float_t height = _element->GetHeight();
			float_t rotation = _element->GetRotation();
			// From the UI::Element pivot and position, get the 4 vertices which
			// make up the box, relative to the pivot.
			std::vector<glm::vec2> points
				= Element::VerticesFromPivot(_element->GetPivot(), origin, width, height);

			// Clear any previous vertex data.
			_vertices.clear();
			// Rotate all 4 vertices around the origin (UI::Element pivot position).
			_vertices.push_back(rotatePoint(points[0], origin, glm::radians(rotation)));
			_vertices.push_back(rotatePoint(points[1], origin, glm::radians(rotation)));
			_vertices.push_back(rotatePoint(points[2], origin, glm::radians(rotation)));
			_vertices.push_back(rotatePoint(points[3], origin, glm::radians(rotation)));
		}

		bool BoxCollider::ContainsPoint(glm::vec2 position)
		{
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

			// If either of the triangles are colliding, return success.
			return (result1 || result2) ? true : false;
		}

		glm::vec2 BoxCollider::rotatePoint(glm::vec2 point, glm::vec2 origin, float_t angle)
		{
			// Translate point to be relative to origin, i.e. origin is treated as (0,0).
			point -= origin;
			// Apply the rotation to the point.
			glm::vec2 result { (point.x * glm::cos(-angle)) - (point.y * glm::sin(-angle)),
				(point.x * glm::sin(-angle)) + (point.y * glm::cos(-angle)) };
			// Undo the origin translation.
			return result + origin;
		}

		float_t BoxCollider::signedDistance(glm::vec2 p, glm::vec2 a, glm::vec2 b)
		{
			// Compute the cross product of the point and the two triangle points which make a line
			// for it to intercept.
			// This essentially computes the inverse gradient of the line, then checks
			// which side of the line the given point is on.
			// This basically computes how far along A-B line you must move (optionally before
			// changing direction) to get to Point.
			return (p.x - b.x) * (a.y - b.y) - (a.x - b.x) * (p.y - b.y);
		}

	}

}