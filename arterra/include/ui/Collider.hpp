#pragma once

#include "PCH.hpp"
#include <glm/glm.hpp>

namespace arterra {
	
	namespace UI {

		/**
		 * @brief Enum specifying which type of collider
		 * the `UI\::Element` has.
		 */
		enum class ColliderType { Box = 0, Circle = 1 };

		class Element;

		/**
		 * @brief BASE Collider class which is inherited by all different
		 * colliders. This class provides the universally required
		 * `GenerateCollider()` and `ContainsPoint()` methods, which are
		 * the only ones needed to be accessed in the `UI\::Manager`.
		 */
		class Collider {

		public:
			/// ctor
			Collider();
			/**
			 * @brief This ctor is called by an inherited collider class, such as
			 * `BoxCollider`, to set the `UI\::Element` pointer which is used
			 * when calling `GenerateCollider()`.
			 * @param element Pointer to the `UI\::Element` this collider is part of.
			 */
			Collider(Element* element);

			/**
			 * @brief Generates the data required for the collider to
			 * properly calculate collisions.
			 * `UI\::Element` calls this method when the transform gets modified.
			 */
			virtual void GenerateCollider() {};
			/**
			 * @brief Checks whether a point lies within this collider.
			 * @param position The position of the point to check.
			 * @return `True` if point lies within the collider, otherwise `False`.
			 */
			virtual bool ContainsPoint(glm::vec2 position) { return false; }

		protected:
			/// @brief Pointer to the `UI\::Element`, which is required to get 
			/// the necessary data for generation of the collider.
			Element* _element;
		};

		/**
		 * @brief BOX Collider class which calculates collisions based off
		 * the 4 vertices which define the rectangle. Requires access to
		 * `position`, `width`, `height`, `rotation` properties of the `UI\::Element`.
		 */
		class BoxCollider : public Collider {

		public:
			/// ctor
			BoxCollider();
			/**
			 * @brief This ctor is called by `UI\::Element` when it is instantiated.
			 * @param element Pointer to the `UI\::Element` this collider is part of.
			 */
			BoxCollider(Element* element);

			/**
			 * @brief Generates the data required for the collider to
			 * properly calculate collisions.
			 * `UI\::Element` calls this method when the transform gets modified.
			 */
			virtual void GenerateCollider();
			/**
			 * @brief Checks whether a point lies within this collider.
			 * @param position The position of the point to check.
			 * @return `True` if point lies within the collider, otherwise `False`.
			 */
			virtual bool ContainsPoint(glm::vec2 position);

		private:
			/// @brief List of vertices making up the box.
			/// [0] = pos, [1] = width-corner, [2] = height-corner, [3] = opposite pos.
			std::vector<glm::vec2> _vertices;

			/**
			 * @brief Used to calculate the Barycentric coordinates for
			 * the two triangles which make up this box collider.
			 * @param p The position of the point P to check.
			 * @param a One of the points of the triangle.
			 * @param b Another point of the triangle.
			 * @return The signed distance from `A` towards `B` required to
			 * reach `P`.
			 */
			float_t signedDistance(glm::vec2 p, glm::vec2 a, glm::vec2 b);
			/**
			 * @brief Used to rotate a point around another point when generating
			 * the collider.
			 * @param point The point to rotate.
			 * @param origin The origin around which to rotate the point.
			 * @param angle The amount to rotate the point by, in clock-wise motion,
			 * in `radians`.
			 */
			glm::vec2 rotatePoint(glm::vec2 point, glm::vec2 origin, float_t angle);
		};

	}

}