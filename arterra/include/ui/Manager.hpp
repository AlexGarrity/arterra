#pragma once

#include "PCH.hpp"
#include <SFML/Window/Event.hpp>

#include "renderer/Renderer.hpp"
#include "renderer/ShaderManager.hpp"
#include "window/Input.hpp"
#include "ui/Element.hpp"

namespace arterra {
	
	namespace UI {

		/**
		 * @brief UI Manager which is responsible for all things UI-related.
		 * Provides the `CreateElement()`, `DestroyElement()`, and
		 * `GetElement()` methods required for manipulating `UI\::Elements`.
		 * Also provides the `Update()` and `Render()` methods to be called from
		 * the current `GameState` to process UI-related logic.
		 */
		class Manager {

		public:
			/// ctor
			Manager();
			/**
			 * @brief This ctor sets the pointers up to point to the already
			 * initialised classes (of which there is only one instance off).
			 * @param shaderManager A pointer to the ShaderManager instantiated in
			 * the current GameState.
			 * @param renderer A pointer to the Renderer instantiated in the
			 * current Engine.
			 * @param event A reference to the sf::Event, part of the Window class
			 * instantiated in the current Engine.
			 * @param input A pointer to the Input instantiated in the current Engine.
			 */
			Manager(ShaderManager* shaderManager, Renderer* renderer, sf::Event& event, Input* input);
			/**
			 * @brief Creates a new `UI\::Element` which exists in the current
			 * `GameState`.
			 * @param identifier The unique identifier for this element.
			 * @param element The element to create as a copy.
			 */
			void CreateElement(std::string identifier, Element element);
			/**
			 * @brief Destroys an existing `UI\::Element` which exists in the
			 * current `GameState`.
			 * @param identifier The unique identifier of the element.
			 */
			void DestroyElement(std::string identifier);

			/**
			 * @brief Gets a pointer to an existing `UI\::Element` in the current 
			 * `GameState`.
			 * @param identifier The unique identifier of the element.
			 * @return `Pointer` to the specified `UI\::Element` stored in this class.
			 */
			Element* GetElement(std::string identifier);
			/**
			 * @brief Gets a pointer to a map of all existing `UI\::Elements` stored
			 * in this class.
			 * @return `Pointer` to the member `_elements` of this class.
			 */
			inline std::unordered_map<std::string, Element>* GetElements() { return &_elements; }

			/**
			 * @brief Runs all the update logic for all `UI\::Elements` in this `GameState`.
			 * This method gets called by the current `GameState::Update()`.
			 */
			void Update();
			/**
			 * @brief Runs all the rendering logic for all `UI\::Elements` in this `GameState`.
			 * This method is called by the current `GameState::Render()`.
			 * This method calls the `Engine::_renderer` class for the actual `glDraw` calls.
			 */
			void Render();

		private:
			/// @brief Map of all `UI\::Elements` in the current `GameState`.
			/// Each element has a unique `identifier` string.
			std::unordered_map<std::string, Element> _elements;

			/// @brief Pointer to the `ShaderManager`, which is required to set the
			/// material/shader properties for a given `UI\::Element`.
			ShaderManager* _shaderManager;
			/// @brief Pointer to the `Renderer`, which is required for the draw calls.
			Renderer* _renderer;
			/// @brief Pointer to the `Input`, which is required to detect mouse-click events.
			Input* _input;
			/// @brief Pointer to the `Event`, part of `Window`, to detect mouse-movement events.
			sf::Event* _event;
		};

	}

}