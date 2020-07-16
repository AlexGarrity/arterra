#include "renderer/Camera.hpp"

namespace arterra {

	ViewFrustum::ViewFrustum() {}

	// https://www.gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf
	void ViewFrustum::Update(glm::mat4& vpMat)
	{
		// Left clipping plane
		_planes[0].normal.x = vpMat[0][3] + vpMat[0][0];
		_planes[0].normal.y = vpMat[1][3] + vpMat[1][0];
		_planes[0].normal.z = vpMat[2][3] + vpMat[2][0];
		_planes[0].d = vpMat[3][3] + vpMat[3][0];
		// Right clipping plane
		_planes[1].normal.x = vpMat[0][3] - vpMat[0][0];
		_planes[1].normal.y = vpMat[1][3] - vpMat[1][0];
		_planes[1].normal.z = vpMat[2][3] - vpMat[2][0];
		_planes[1].d = vpMat[3][3] - vpMat[3][0];
		// Top clipping plane
		_planes[2].normal.x = vpMat[0][3] - vpMat[0][1];
		_planes[2].normal.y = vpMat[1][3] - vpMat[1][1];
		_planes[2].normal.z = vpMat[2][3] - vpMat[2][1];
		_planes[2].d = vpMat[3][3] - vpMat[3][1];
		// Bottom clipping plane
		_planes[3].normal.x = vpMat[0][3] + vpMat[0][1];
		_planes[3].normal.y = vpMat[1][3] + vpMat[1][1];
		_planes[3].normal.z = vpMat[2][3] + vpMat[2][1];
		_planes[3].d = vpMat[3][3] + vpMat[3][1];
		// Near clipping plane
		_planes[4].normal.x = vpMat[0][3] + vpMat[0][2];
		_planes[4].normal.y = vpMat[1][3] + vpMat[1][2];
		_planes[4].normal.z = vpMat[2][3] + vpMat[2][2];
		_planes[4].d = vpMat[3][3] + vpMat[3][2];
		// Far clipping plane
		_planes[5].normal.x = vpMat[0][3] - vpMat[0][2];
		_planes[5].normal.y = vpMat[1][3] - vpMat[1][2];
		_planes[5].normal.z = vpMat[2][3] - vpMat[2][2];
		_planes[5].d = vpMat[3][3] - vpMat[3][2];

		for (auto& p : _planes) {
			float l = 1.0f / glm::length(p.normal);
			p.normal *= l;
			p.d *= l;
		}
	}

	float ViewFrustum::Plane::DistanceToPoint(glm::vec3 point) { return glm::dot(point, normal) + d; }

	bool ViewFrustum::PointInFrustum(glm::vec3 point)
	{
		for (auto& p : _planes) {
			if (p.DistanceToPoint(point) < 0.0f)
				return false;
		}
		return true;
	}

	bool ViewFrustum::ChunkInFrustum(WorldPosition pos)
	{
		auto getVP = [&](float x, float y, float z) {
			auto res = pos;
			res._x += (x > 0) ? 16 : 0;
			res._y += (y > 0) ? 16 : 0;
			res._z += (z > 0) ? 16 : 0;
			return glm::vec3 { res._x, res._y, res._z };
		};

		auto getVN = [&](float x, float y, float z) {
			auto res = pos;
			res._x += (x < 0) ? 16 : 0;
			res._y += (y < 0) ? 16 : 0;
			res._z += (z < 0) ? 16 : 0;
			return glm::vec3 { res._x, res._y, res._z };
		};

		bool result = true;
		for (auto& plane : _planes) {
			if (plane.DistanceToPoint(getVP(plane.normal.x, plane.normal.y, plane.normal.z)) < 0) {
				return false;
			} else if (plane.DistanceToPoint(getVN(plane.normal.x, plane.normal.y, plane.normal.z)) < 0) {
				result = true;
			}
		}
		return result;
	}

	Camera::Camera()
	{
		// Create a new perspective projection
		_projection = glm::perspective(glm::radians(55.0f), 16.f / 9.f, 0.1f, 1000.0f);
	}

	void Camera::Update(Window& window, float_t deltaTime)
	{
		// Update the ortho projection in case the window gets resized.
		_guiProjection = glm::ortho(0.0f, static_cast<float>(window.GetWidth()),
			0.0f, static_cast<float>(window.GetHeight()));
		
		_transform.Update();
		

		// Update the view projection to account for movement
		_view = glm::mat4 { 1.0f };
		_viewProjection = glm::mat4 { 1.0f };
		_view = glm::lookAt(_transform.Position(), _transform.Position() + _transform.Forward(), _transform.Up());
		_viewProjection = _projection * _view;
		_viewFrustum.Update(_viewProjection);
	}

}