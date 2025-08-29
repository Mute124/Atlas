
//#include <project.h>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/uuid/name_generator_md5.hpp>
#include <fstream>
#include <iostream>
#include <ios>
#include <iosfwd>
#include <string>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include <core/AtlasEngine.h>

class Person {
private:
	std::string name;
	int age;
	double height;

	// Make Boost.Serialization a friend to access private members
	friend class boost::serialization::access;

	// Method to serialize and deserialize the object
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& name;
		ar& age;
		ar& height;
	}

public:
	// Constructors
	Person(const std::string& n = "", int a = 0, double h = 0.0)
		: name(n), age(a), height(h) {}

	// Display the Person's data
	void display() const {
		std::cout << "Name: " << name << ", Age: " << age
			<< ", Height: " << height << " meters" << std::endl;
	}
};
using namespace Atlas;

int main(int argc, char* argv[]) {

	// Serialize the object to a file
/*	Person p1("John Doe", 30, 1.75);
	{
		boost::uuids::uuid namespace_id = boost::uuids::string_generator()("my_namespace");
		boost::uuids::uuid uuid = boost::uuids::name_generator_md5(namespace_id)(&p1, sizeof(Person));

		std::ofstream ofs("C:\\Dev\\Techstorm-v5\\game\\data\\" + boost::uuids::to_string(uuid) + ".dat", std::ios_base::binary | std::ios_base::app);
		boost::archive::text_oarchive oa(ofs);
		oa.save_binary(&p1, sizeof(Person));  // Save the serialized data in binary format
	}

	// Serialize the object to a file
	Person p3("John roe", 260, 1.4435);
	{
		boost::uuids::uuid uuid = boost::uuids::random_generator()();

		std::ofstream ofs("C:\\Dev\\Techstorm-v5\\game\\data\\" + boost::uuids::to_string(uuid) + "person.dat", std::ios_base::binary | std::ios_base::app);
		boost::archive::text_oarchive oa(ofs);
		
		oa.save_binary(&p3, sizeof(Person));  // Save the serialized data in binary format
		//oa << p3;  // Serialize
	}

	// Deserialize the object from the file
	Person p2;
	{
	//	std::ifstream ifs("C:\\Dev\\Techstorm-v5\\game\\data\\person.dat");
//		boost::archive::text_iarchive ia(ifs);
	//	ia >> p2;  // Deserialize
	}*/

	GameWindowSettings windowSettings{};
	windowSettings.enableEventPolling = true;

	WindowDescription windowDesc{};
	windowDesc.windowRectangle.width = 800;
	windowDesc.windowRectangle.height = 600;
	windowDesc.title = "Atlas";
	//windowSettings.windowDescription = windowDesc;
	
	IGameWindow* gameWindow = nullptr;

#ifdef ATLAS_USE_GLFW
	gameWindow = new GLFWGameWindow("Atlas", 800, 600, NULL, 60, "", windowSettings);
#elif defined ATLAS_USE_SDL2

	gameWindow = new SDLGameWindow();

	gameWindow->setWindowTitle("Atlas");
	gameWindow->setWindowSize(800, 600);
	gameWindow->setWindowPosition(0, 0);
	gameWindow->setTargetFPS(60);

#endif

	std::shared_ptr<VulkanRenderer> renderer = std::make_shared<VulkanRenderer>(gameWindow);
	std::shared_ptr<GameThreader> gameThreader = std::make_shared<GameThreader>();

	//EngineModulesInfo modulesInfo = EngineModulesInfo{ 
	//	new VulkanRenderer(gameWindow),
	//	new GameThreader()
	//};

	bool beginLoop = false;
	AtlasSettings settings{};
	//modulesInfo, settings
	
	std::unique_ptr<AtlasEngine> atlas = std::make_unique<AtlasEngine>();

	atlas->setGameThreader(gameThreader);
	
	atlas->setRenderer(renderer);

	atlas->threadEngine(3);

	atlas->init();

	atlas->mGameThreader->addScheduler("Rendering", 1);
	atlas->mGameThreader->addScheduler("Update", 1);

	// why the fuck is this being done before it gets scheduled?
	auto rendererInitFuture = atlas->mGameThreader->getScheduler("Rendering")->schedule(
		[renderer, &beginLoop]() {
			//std::cout << "Hello from the rendering thread" << std::endl;
			renderer->init();
			renderer->mainGameWindow->open((SDL_WindowFlags)(SDL_WINDOW_VULKAN));
			beginLoop = true;
		}
	);

	//atlas->run();

	while (!beginLoop) {

	}

	while (!atlas->shouldExit() && renderer->isInitialized()) {

		// Tell the renderer to update (this automatically happens after everything is ready).
		atlas->mGameThreader->getScheduler("Rendering")->schedule(
			[&]() {
				renderer->update();
			}
		);

		atlas->update();
	}

	return 0;
}
