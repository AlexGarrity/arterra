#pragma once
#include "PCH.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model/Model.hpp"
#include "renderer/Camera.hpp"
#include "renderer/ShaderManager.hpp"
#include "texture/Texture.hpp"
#include "renderer/VertexArray.hpp"

namespace arterra {
	class Renderer {
	public:
		Renderer(Camera* camera);

		// Update the renderer to account for input
		void Update();

		// Draw a rainbow cube
		void DrawTestCube();

	private:
		GLint _viewProjectionUniform;
		Camera* _camera;

		Model _cubeModel;
		TextureHandle _textureHandle;
	};

}