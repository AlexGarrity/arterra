#pragma once

namespace arterra {

	struct WorldPosition;
	
	class ViewFrustum {
		struct Plane {
			glm::vec3 normal;
			float d;

			float DistanceToPoint(glm::vec3 point);
		};

	public:
		ViewFrustum();
		void Update(glm::mat4& viewProjectionMatrix);

		bool PointInFrustum(glm::vec3 point);
		bool ChunkInFrustum(const WorldPosition& pos);

	private:
		std::array<Plane, 6> _planes;
	};
	
}