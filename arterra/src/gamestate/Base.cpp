#include "gamestate/Base.hpp"

arterra::gamestate::Base::Base(Engine* engine)
	: _engine{ engine } { Logger::Debug( "Created a new init state"); }

arterra::gamestate::Base::~Base() {}
