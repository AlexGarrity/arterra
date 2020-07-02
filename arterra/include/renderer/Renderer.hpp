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
#include "renderer/VertexArray.hpp"

namespace arterra {
    class Renderer {
        public:
            Renderer(Camera &camera);
			
            // Update the renderer to account for input
            void Update();

            // Draw a rainbow cube
            void DrawTestCube();

        private:
            
			VertexArray vao;
			VertexBuffer vbP;
			VertexBuffer vbC;

            // Test cube vertices
            std::vector<float> _positions = {
				// FRONT
                0.5f, 0.5f, 0.5f, 		// 1
				0.5f, -0.5f, 0.5f, 	// 2
				-0.5f, -0.5f, 0.5f, 	// 3
				
				0.5f, 0.5f, 0.5f, 		// 1
				-0.5f, -0.5f, 0.5f, 	// 3
				-0.5f, 0.5f, 0.5f, 	// 4
				
				// BACK
				-0.5f, 0.5f, -0.5f, 	// 8
				-0.5f, -0.5f, -0.5f, 	// 7
				0.5f, -0.5f, -0.5f, 	// 6
				
				-0.5f, 0.5f, -0.5f, 	// 8
				0.5f, -0.5f, -0.5f, 	// 6
				0.5f, 0.5f, -0.5f, 	// 5
				
				// TOP
				0.5f, 0.5f, -0.5f, 	// 5
				0.5f, 0.5f, 0.5f, 		// 1
				-0.5f, 0.5f, 0.5f, 	// 4
				
				0.5f, 0.5f, -0.5f, 	// 5
				-0.5f, 0.5f, 0.5f, 	// 4
				-0.5f, 0.5f, -0.5f, 	// 8
				
				// BOTTOM
				0.5f, -0.5f, 0.5f, 	// 2
				0.5f, -0.5f, -0.5f, 	// 6
				-0.5f, -0.5f, -0.5f, 	// 7
				
				0.5f, -0.5f, 0.5f, 	// 2
				-0.5f, -0.5f, -0.5f, 	// 7
				-0.5f, -0.5f, 0.5f, 	// 3
				
				// RIGHT
				0.5f, 0.5f, -0.5f, 	// 5
				0.5f, -0.5f, -0.5f, 	// 6
				0.5f, -0.5f, 0.5f, 	// 2
				
				0.5f, 0.5f, -0.5f, 	// 5
				0.5f, -0.5f, 0.5f, 	// 2
				0.5f, 0.5f, 0.5f, 		// 1
				
				// LEFT
				-0.5f, 0.5f, 0.5f, 	// 4
				-0.5f, -0.5f, 0.5f, 	// 3
				-0.5f, -0.5f, -0.5f, 	// 7
				
				-0.5f, 0.5f, 0.5f, 	// 4
				-0.5f, -0.5f, -0.5f, 	// 7
				-0.5f, 0.5f, -0.5f, // 8
				
            };
			
			std::vector<float> _colours = {
					1.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 1.0f,
					1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f,
					1.0f, 1.0f, 1.0f,
					1.0f, 0.0f, 1.0f,
					0.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 0.0f,
					1.0f, 0.0f, 1.0f,
					1.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f,
					1.0f, 0.0f, 1.0f,
					0.0f, 1.0f, 0.0f,
					1.0f, 1.0f, 0.0f,
					0.0f, 1.0f, 1.0f,
					0.0f, 1.0f, 0.0f,
					0.0f, 1.0f, 1.0f,
					0.0f, 0.0f, 1.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					1.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f,
					0.0f, 0.0f, 1.0f,
					0.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,
					0.0f, 1.0f, 1.0f,
					1.0f, 0.0f, 1.0f	
			};
		
		GLint _viewProjectionUniform;
		Camera* _camera;
		ShaderManager _shaderManager;

    };

    
}