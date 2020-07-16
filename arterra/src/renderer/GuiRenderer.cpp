#include "renderer/GuiRenderer.hpp"

namespace arterra {
	
	GuiRenderer::GuiRenderer(Renderer& renderer, ShaderManager& shaderManager)
		: _renderer(&renderer), _shaderManager(&shaderManager), _renderables() {
	}
		
	void GuiRenderer::AddElement(UI::Element* element) {
		_renderables.push_back(element);
	}
	
	void GuiRenderer::Render() {
		
		glDisable(GL_DEPTH_TEST);
		
		for (auto renderable : _renderables) {

			if (true) {
				// Get all shader parameters for this gui-element's material.
				auto parameters = renderable->GetMaterial().GetParameters();
				_shaderManager->UseShader("gui-fancy");
				for (auto param: parameters) {
					switch (param._type)
					{
					case UI::ShaderParameter::Type::Int :
						_shaderManager->ActiveProgram().SetUniform(param._name, param._value.i);
						break;
					case UI::ShaderParameter::Type::Float :
						_shaderManager->ActiveProgram().SetUniform(param._name, param._value.f);
						break;
					case UI::ShaderParameter::Type::Vec2 :
						_shaderManager->ActiveProgram().SetUniform(param._name, param._value.v2);
						break;
					case UI::ShaderParameter::Type::Vec3 :
						_shaderManager->ActiveProgram().SetUniform(param._name, param._value.v3);
						break;
					case UI::ShaderParameter::Type::Vec4 :
						_shaderManager->ActiveProgram().SetUniform(param._name, param._value.v4);
						break;
					case UI::ShaderParameter::Type::Mat4 :
						_shaderManager->ActiveProgram().SetUniform(param._name, param._value.m4);
						break;
					}
				}
				
				// Bind and draw the gui-element.
				renderable->GetMesh().Bind();
				_renderer->DrawTriangles(renderable->GetMesh().GetVertexCount());
			}		
		}
		
	}
	
}