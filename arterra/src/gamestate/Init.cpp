#include "gamestate/Init.hpp"

namespace arterra {

	namespace gamestate {

		Init::Init(Engine* engine)
			: Base(engine)
		{
		}

		void Init::Input(const float deltaTime) {}

		void Init::Update(const float deltaTime) {}

		void Init::Render(const float deltaTime) {}

	}

}