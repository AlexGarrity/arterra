#pragma once

#include "PCH.hpp"

#include "Engine.hpp"

namespace arterra {

    namespace gamestate {

        class Base {
            public:
                Base(std::shared_ptr<Engine> engine) : _engine {engine} {
                    Logger::Get().Log(Logger::Debug, "Created a new init state");
                };

                ~Base() {}

                virtual void Input(float_t deltaTime) = 0;
                virtual void Update(float_t deltaTime) = 0;
                virtual void Render(float_t deltaTime) = 0;

            protected:
                std::shared_ptr<Engine> _engine;
        };

    }

}