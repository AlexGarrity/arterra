#pragma once

#include "PCH.hpp"
#include "renderer/Renderer.hpp"
#include "util/Time.hpp"

namespace arterra {

    class gamestate::Base;

    class Engine {

        public:
            Engine();
            ~Engine();

            void Run();

            const std::shared_ptr<gamestate::Base> GetState() const;
            void PushState(std::shared_ptr<gamestate::Base> state);
            void PopState();

            const inline bool StateQueueEmpty() const { return _stateQueue.empty(); }
            inline void SetShouldExit(bool b) { _shouldExit = b; }


        private:
            bool _shouldExit = false;
            std::queue<std::shared_ptr<gamestate::Base>> _stateQueue;

            Renderer *_renderer;
            Time *time;

    };

}