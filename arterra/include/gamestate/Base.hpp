#pragma once

#include "PCH.hpp"

#include "Engine.hpp"

namespace arterra {

	namespace gamestate {

		class Base {
		public:
			explicit Base(Engine* engine);
			virtual ~Base();

			virtual void Input(float deltaTime) = 0;
			virtual void Update(float deltaTime) = 0;
			virtual void Render(float deltaTime) = 0;

		protected:
			Engine* _engine;
		};

	}

}