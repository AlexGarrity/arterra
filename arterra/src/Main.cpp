#include "Engine.hpp"
#include "gamestate/Test.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace arterra;

int main(int argc, char** argv)
{
	// Set correct filepath
	std::filesystem::current_path(std::filesystem::path(argv[0]).parent_path());
	// Create a new engine instance
	Engine* engine = new Engine();
	// Push the test state onto the engine
	engine->PushState(new gamestate::Test(engine));
	// Run the engine
	engine->Run();
	// Execution finished, delete the engine (should be gamestate::deinit)
	delete engine;
	// Success
	return 0;
}