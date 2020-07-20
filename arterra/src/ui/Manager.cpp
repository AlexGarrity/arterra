#include "ui/Manager.hpp"

namespace arterra {

	namespace UI {

		Manager::Manager(ShaderManager* shaderManager, Renderer* renderer, sf::Event& event, Input* input)
			: _shaderManager(shaderManager)
			, _renderer(renderer)
			, _event(&event)
			, _input(input)
			, _elements()
		{
		}

		void Manager::CreateElement(std::string identifier, Element element)
		{
			// Make sure that the identifier isn't already taken.
			auto eElement = _elements.find(identifier);
			if (eElement != _elements.end()) {
				Logger::Get().Log(Logger::Error, "[UI] Unable to create new UI element `", identifier,
					"` since it already exists!");
				return;
			}

			// First create a copy of the element within this class.
			_elements[identifier] = element;

			// Then create the mesh and collider on the "proper" copy,
			// as well as register mouseEvents.
			_elements[identifier].CreateMesh();
			_elements[identifier].CreateCollider();
			auto e = &_elements[identifier];
			_elements[identifier].OnMouseEnter
				= [e]() { e->GetMaterial().UpdateParameter("u_ColourTint", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)); };
			_elements[identifier].OnMouseLeave
				= [e]() { e->GetMaterial().UpdateParameter("u_ColourTint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); };
			_elements[identifier].OnMouseClick
				= [e]() { e->GetMaterial().UpdateParameter("u_ColourTint", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)); };
		}

		void Manager::DestroyElement(std::string identifier)
		{
			auto element = _elements.find(identifier);
			if (element == _elements.end()) {
				Logger::Get().Log(
					Logger::Warning, "[UI] Trying to destroy UI element `", identifier, "' which does not exist!");
				return;
			}
			_elements.erase(element);
		}

		Element* Manager::GetElement(std::string identifier)
		{
			auto element = _elements.find(identifier);
			if (element == _elements.end()) {
				Logger::Get().Log(
					Logger::Error, "[UI] Trying to get UI element `", identifier, "' which does not exist!");
				return nullptr;
			}
			return &element->second;
		}

		void Manager::Update()
		{
			// First check for mouse movement.
			if (_event->type == sf::Event::MouseMoved) {
				for (auto it = _elements.begin(); it != _elements.end(); it++) {
					// Iterate through each UI element and update it.
					it->second.Update(glm::vec2 { _event->mouseMove.x, (720 - _event->mouseMove.y) }, -1);
				}
			}

			// Then check for mouse button presses.
			auto mouseData = _input->PollMouseBind("primary-button");
			if (mouseData._timePressed == 0.0f && mouseData._isActive) {
				for (auto it = _elements.begin(); it != _elements.end(); it++) {
					// Iterate through each UI element and update it.
					it->second.Update(glm::vec2 { _event->mouseMove.x, (720 - _event->mouseMove.y) }, 0);
				}
			}
		}

		void Manager::Render()
		{
			for (auto it = _elements.begin(); it != _elements.end(); it++) {
				// Disable depth test.
				glDisable(GL_DEPTH_TEST);

				// Update transform in case it's changed.
				it->second.UpdateTransform();

				// Get all shader parameters for this gui-element's material.
				auto parameters = it->second.GetMaterial().GetParameters();
				_shaderManager->UseShader(it->second.GetMaterial().GetShaderIdentifier());
				for (auto param = parameters.begin(); param != parameters.end(); param++) {
					switch (param->_type) {
						case UI::ShaderParameter::Type::Int:
							_shaderManager->ActiveProgram().SetUniform(param->_name, param->_value.i);
							break;
						case UI::ShaderParameter::Type::Float:
							_shaderManager->ActiveProgram().SetUniform(param->_name, param->_value.f);
							break;
						case UI::ShaderParameter::Type::Vec2:
							_shaderManager->ActiveProgram().SetUniform(param->_name, param->_value.v2);
							break;
						case UI::ShaderParameter::Type::Vec3:
							_shaderManager->ActiveProgram().SetUniform(param->_name, param->_value.v3);
							break;
						case UI::ShaderParameter::Type::Vec4:
							_shaderManager->ActiveProgram().SetUniform(param->_name, param->_value.v4);
							break;
						case UI::ShaderParameter::Type::Mat4:
							_shaderManager->ActiveProgram().SetUniform(param->_name, param->_value.m4);
							break;
					}
				}

				// Bind and draw the UI element.
				it->second.GetMesh().Bind();
				_renderer->DrawTriangles(it->second.GetMesh().GetVertexCount());
			}
		}

	}

}