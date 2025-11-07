
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

//#include <core/AtlasEngine.h>

#include <graphics/backend/VKDevice.h>
#include <debugging/Logging.h>
#include <io/IOManager.h>

#include <haptics/HapticDevice.h>
#include <haptics/HapticEffect.h>

//class Person {
//private:
//	std::string name;
//	int age;
//	double height;
//
//	// Make Boost.Serialization a friend to access private members
//	friend class boost::serialization::access;
//
//	// Method to serialize and deserialize the object
//	template<class Archive>
//	void serialize(Archive& ar, const unsigned int version) {
//		ar& name;
//		ar& age;
//		ar& height;
//	}
//
//public:
//	// Constructors
//	Person(const std::string& n = "", int a = 0, double h = 0.0)
//		: name(n), age(a), height(h) {}
//
//	// Display the Person's data
//	void display() const {
//		std::cout << "Name: " << name << ", Age: " << age
//			<< ", Height: " << height << " meters" << std::endl;
//	}
//};
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

	const std::string logFilePath = std::format("logs/{}", SpdlogLogger::GenerateLogFileName());

	SpdlogLogger logger = SpdlogLogger("Atlas", ATLAS_DEFAULT_SPDLOG_LOG_PATTERN, logFilePath);
	logger.init();
	logger.setThisAsDefaultLogger();
	logger.info(logFilePath);

	FileManager::Options options;
	options.fileTTL = std::chrono::seconds(10);
	options.evictionCheckInterval = std::chrono::seconds(5);
	options.bStartJanitor = true;

	FileManager fileManager(options);
	fileManager.addExtensionLoader(".spv", [](std::filesystem::path path, std::shared_ptr<FileData>& data, std::shared_ptr<FileRecord> record) {
		// open the file. With cursor at the end
		std::ifstream file(path, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			return false;
		}

		// find what the size of the file is by looking up the location of the cursor
		// because the cursor is at the end, it gives the size directly in bytes
		size_t fileSize = (size_t)file.tellg();

		// spirv expects the buffer to be on uint32, so make sure to reserve a int
		// vector big enough for the entire file
		std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

		// put file cursor at beginning
		file.seekg(0);

		// load the entire file into the buffer
		file.read((char*)buffer.data(), fileSize);

		// now that the file is loaded into the buffer, we can close it
		file.close();

		std::vector<FileData::Byte> bytes;

		for (size_t i = 0; i < buffer.size(); i++) {
			bytes.push_back((FileData::Byte)buffer[i]);
		}

		data = std::make_shared<FileData>(std::move(buffer));
		//data->print();
		
		return true;
	});

	fileManager.registerDirectory("F:/dev/AtlasIOPrototype/assets");

	InfoLog(std::format("Registered Files: {}", fileManager.getRegisteredCount()));

	//std::cout << "Registered files: " << fileManager.getRegisteredCount() << "\n";

	// open a file (lazy load)
	auto data = fileManager.openFile("F:/dev/AtlasIOPrototype/assets/TestModel.obj");
	if (data) {
		//std::cout << "Loaded bytes: " << data.get()->bytes.size() << "\n";
		// use data->bytes...
	}
	else {
		//std::cout << "Failed to load file\n";
	}

	// Setup the game window (this needs to be done before the rendering device is created)
	std::unique_ptr<SDLGameWindow> gameWindow = std::make_unique<SDLGameWindow>();

	gameWindow->setWindowTitle("Atlas");
	gameWindow->setWindowSize(800, 600);
	gameWindow->setWindowPosition(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	gameWindow->setTargetFPS(60);

	gameWindow->init(SDL_INIT_VIDEO | SDL_INIT_HAPTIC);
	gameWindow->open((SDL_WindowFlags)(SDL_WINDOW_VULKAN));

	RenderingBackend::APIVersion renderingAPIVersion;
	renderingAPIVersion.major = 1;
	renderingAPIVersion.minor = 3;
	renderingAPIVersion.patch = 0;

	auto renderingDevice = std::make_unique<VulkanRenderingBackend>();

	renderingDevice->setAPIVersion(renderingAPIVersion);
	renderingDevice->setApplicationName("Example Application");

	renderingDevice->init(gameWindow.operator->());

	renderingDevice->testLoad(fileManager);
	//std::shared_ptr<HapticDevice> hapticDevice = std::make_shared<HapticDevice>(0);
	//hapticDevice->open();

	//MonoRumbleHapticEffect effect = MonoRumbleHapticEffect();
	//effect.upload(hapticDevice);
	//effect.play(hapticDevice);

	while (!gameWindow->shouldClose()) {
		gameWindow->update();

		renderingDevice->beginDrawingMode();
		renderingDevice->draw();
		renderingDevice->endDrawingMode();
	}

	// explicit unload attempt
	bool unloaded = fileManager.unloadFile("F:/dev/AtlasIOPrototype/assets/TestModel.obj");
	std::cout << "Explicit unload result: " << unloaded << "\n";

	renderingDevice->shutdown();
	gameWindow->cleanup();
	//effect.destroy(hapticDevice);
	//hapticDevice->close();

	return 0;
}
