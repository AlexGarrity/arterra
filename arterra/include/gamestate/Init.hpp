#pragma once

#include "PCH.hpp"

#include "gamestate/Base.hpp"

namespace arterra {

    namespace gamestate {

        class Init : public Base {
        public:
            Init(std::shared_ptr<Engine> engine) : Base(engine) {};

            virtual void Input(float_t deltaTime);
            virtual void Update(float_t deltaTime);
            virtual void Render(float_t deltaTime);

            private:

        };

    }

}