#pragma once

#include "PCH.hpp"

#include "gamestate/Base.hpp"

namespace arterra {

	namespace gamestate {

		class Init : public Base {
		public:
			explicit Init(Engine* engine);

			void Input(float deltaTime) override;
			void Update(float deltaTime) override; 
			void Render(float deltaTime) override;

		private:
		};

	}

}