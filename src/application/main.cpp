#include <fstream>
#include <iostream>
#include <ios>
#include <iosfwd>
#include <string>

#include <graphics/vulkan/VKDevice.h>
#include <debugging/Logging.h>
#include <io/IOManager.h>
#include <core/platform/Platform.h>
#include <SDL2/SDL.h>

#include <imgui/backends/imgui_impl_sdl2.h>
#include <SDL2/SDL_events.h>
using namespace Atlas;

int main(int argc, char* argv[]) {
	const std::string logFilePath = std::format("logs/{}", SpdlogLogger::GenerateLogFileName());

	SpdlogLogger logger = SpdlogLogger("Atlas", ATLAS_DEFAULT_SPDLOG_LOG_PATTERN, logFilePath);
	logger.init();
	logger.setThisAsDefaultLogger();

	FileManager::Options options;
	options.fileTTL = std::chrono::seconds(10);
	options.evictionCheckInterval = std::chrono::seconds(5);
	options.bStartJanitor = true;

	PlatformInitInfo platformInitInfo{};
	platformInitInfo.applicationInfo.applicationName = "test";
	platformInitInfo.fileManagerOptions = options;

	Platform platform = Platform(platformInitInfo);

	std::shared_ptr<FileManager> fileManager = platform.getFileManager().lock();
	fileManager->addExtensionLoader(".spv", [](std::filesystem::path path, std::shared_ptr<FileData>& data, std::shared_ptr<FileRecord> record) {
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
		
		return true;
	});
	fileManager->registerDirectory("F:/dev/AtlasIOPrototype/assets");

	InfoLog(std::format("Registered Files: {}", fileManager->getRegisteredCount()));

	// open a file (lazy load)
	auto data = fileManager->openFile("F:/dev/AtlasIOPrototype/assets/TestModel.obj");

	// Setup the game window (this needs to be done before the rendering device is created)
	GameWindow gameWindow = GameWindow(WindowDescription());

	gameWindow.open(SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);

	Version renderingAPIVersion;
	renderingAPIVersion.major = 1;
	renderingAPIVersion.minor = 3;
	renderingAPIVersion.patch = 0;

	auto renderingDevice = std::make_unique<VulkanRenderingBackend>();
	renderingDevice->setAPIVersion(renderingAPIVersion);
	renderingDevice->setApplicationName("Example Application");
	renderingDevice->init(&gameWindow);

	while (!gameWindow.shouldClose()) {
		gameWindow.update();
		
		renderingDevice->beginDrawingMode();
		renderingDevice->draw();
		renderingDevice->endDrawingMode();
	}

	// explicit unload attempt
	bool unloaded = fileManager->unloadFile("F:/dev/AtlasIOPrototype/assets/TestModel.obj");
	std::cout << "Explicit unload result: " << unloaded << "\n";

	renderingDevice->shutdown();
	//gameWindow->cleanup();
	SDL_Quit();
	return 0;
}
