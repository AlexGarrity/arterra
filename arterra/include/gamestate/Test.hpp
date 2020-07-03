#pragma once

#include "PCH.hpp"

#include "gamestate/Base.hpp"

namespace arterra {

    namespace gamestate {

        class Test : public Base {
        public:
            Test(Engine *engine);

            virtual void Input(float_t deltaTime) override;
            virtual void Update(float_t deltaTime) override;
            virtual void Render(float_t deltaTime) override;

            private:
                float _timeToResourceUnload = 5.0f;

        };

    }

}