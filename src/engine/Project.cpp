#include "Project.h"
#include "physics/Physics.h"
#include <raylib.h>
#include "Common.h"
#include "conf/Config.h"
#include "dbg/Logging.h"
#include "fs/FileMeta.h"
#include "fs/FileSystem.h"
#include "fs/FileSystemRegistry.h"
#include "GameSettings.h"
#include "modding/ScriptingAPI.h"
#include "renderer/Renderer.h"
#include "renderer/window/IWindow.h"
#include "renderer/window/Window.h"
#include "renderer/window/WindowDecorations.h"
#include <any>
#include <memory>
#include <string>
#include <sol/types.hpp>
#include "input/Input.h"
#include <thread>

using namespace Atlas;

std::shared_ptr<AtlasEngine> Atlas::BProject::setupAtlas()
{
	return std::shared_ptr<AtlasEngine>(
		new AtlasEngine(
			new ConfigFileRegistry(),
			new ScriptingAPI(),
			new Window(),
			new Renderer(),
			new GameSettings(),
			new FileSystemRegistry(),
			new PhysicsEngine(),
			new InputRegistry(),
			new Logger()
		)
	);
}

/*Atlas::BProject::BProject(std::shared_ptr<AtlasEngine> engine) : IProject()
{
	if (mAtlas == nullptr) {
		mAtlas = engine;
	}
}*/

Atlas::BProject::BProject()  : IProject()
{
	mProject = std::shared_ptr<BProject>(this);

	if (mAtlas == nullptr) {
		mAtlas = setupAtlas();
	}
}



WindowDecorations& Atlas::BProject::getWindowDecorations() { 
	return mWindowDecorations;
}

void Atlas::BProject::setWindowDecorations(WindowDecorations& windowDecorations) {
	mWindowDecorations = windowDecorations; 
}

void Atlas::BProject::preInit() {

	getAtlasEngine()->getLogger()->init(LoggerConfig());
	Log("Registering file load functions...");
	AddFileRegistryLoadFunction("png", [](std::shared_ptr<FileMeta> loadFunc) {
		Image image = LoadImage(loadFunc->path.c_str());

		if (image.data == nullptr) {
			if (loadFunc->filename == "null.png") {
				// This means that the null image was not found, which is a bad bad thing.
				throw std::runtime_error("Null png image not found!");

				// TODO: Figure out how to handle this.
				return std::any_cast<Image>(image);
			}

			std::string nullPath = GetFilePath("null.png");

			image = LoadImage(nullPath.c_str());
		}

		return std::any_cast<Image>(image);
	});

	AddFileRegistryLoadFunction("jpg", [](std::shared_ptr<FileMeta> loadFunc) {

		Image image = LoadImage(loadFunc->path.c_str());

		if (image.data == nullptr) {
			if (loadFunc->filename == "null.jpg") {
				// This means that the null image was not found, which is a bad bad thing.
				throw std::runtime_error("Null jpg image not found!");

				// TODO: Figure out how to handle this.
				return std::any_cast<Image>(image);
			}

			std::string nullPath = GetFilePath("null.jpg");

			image = LoadImage(nullPath.c_str());
		}

		return std::any_cast<Image>(image);
	});

	AddFileRegistryLoadFunction("jpeg", [](std::shared_ptr<FileMeta> loadFunc) {

		Image image = LoadImage(loadFunc->path.c_str());

		if (image.data == nullptr) {
			if (loadFunc->filename == "null.jpeg") {
				// This means that the null image was not found, which is a bad bad thing.
				throw std::runtime_error("Null jpeg image not found!");

				// TODO: Figure out how to handle this.
				return std::any_cast<Image>(image);
			}
			
			std::string nullPath = GetFilePath("null.jpeg");

			image = LoadImage(nullPath.c_str());
		}

		return std::any_cast<Image>(image);
	});

	Log("Done registering file load functions...");

	std::string gameDir = ATLAS_GAME_DIR;

	FileSystemRegistry* registry = getAtlasEngine()->getFileSystemRegistry();
	ConfigFileRegistry* configFileRegistry = getAtlasEngine()->getConfigFileRegistry();

	Log("Initializing file system...");
	registry->init(gameDir.c_str());
	configFileRegistry->init();

//#ifdef ATLAS_ENABLE_MODDDING
//		#ifdef ATLAS_ENABLE_LUA
	this->mLuaLibraries.push_back(sol::lib::base);

	ScriptingAPI* scriptingAPI = getAtlasEngine()->getScriptingAPI();

	scriptingAPI->initializeScripting(this->getLuaLibraries(), this->getLuaFunctions()); // getAtlasEngine()->getLuaLibraries(), getAtlasEngine()->getLuaFunctions()
	scriptingAPI->registerLua();
//		#endif
//#endif
}

void Atlas::BProject::init(int argc, char* argv[]) {

}

void Atlas::BProject::postInit() {
	IWindow* window = getAtlasEngine()->getWindow();
	Renderer* renderer = getAtlasEngine()->getRenderer();
	AllocatedPhysicsResources resources = AllocatedPhysicsResources();
	getAtlasEngine()->getPhysicsEngine()->init(resources);
	window->init(new WindowDecorations());
	renderer->init();
}

void Atlas::BProject::initRenderer() {
}

int Atlas::BProject::run(int argc, char* argv[]) {
	int code = 0;
	
	bool shouldClose = false;
	while (!shouldClose) {
		shouldClose = getAtlasEngine()->getWindow()->shouldClose();

		draw();
	}

	// This is here because it reduces CPU usage (by alot!).
	std::this_thread::yield();
	return code;
}

int Atlas::BProject::update()
{
	int code = 0;
	
	getAtlasEngine()->getRenderer()->updateObjects();
	
	return code;
}

int Atlas::BProject::workingUpdate()
{
	int code = 0;

	getAtlasEngine()->getInputRegistry()->checkAll();

	return code;
}

int Atlas::BProject::prePhysicsUpdate() {
	int code = 0;

	return code;
}

int Atlas::BProject::physicsUpdate() {

	PhysicsEngine* physicsEngine = getAtlasEngine()->getPhysicsEngine();
	physicsEngine->update(1.0f / 60.0f);

	return 0; 
}

int Atlas::BProject::postPhysicsUpdate() { 
	return 0; 
}

int Atlas::BProject::preObjectUpdate() { 
	return 0; 
}

int Atlas::BProject::objectUpdate() { 
	return 0; 
}

int Atlas::BProject::postObjectUpdate() { 
	return 0; 
}

int Atlas::BProject::texture() {
	getAtlasEngine()->getRenderer()->texture(getAtlasEngine()->getRenderer()->mCamera);
	return 0; 
}

int Atlas::BProject::render() { 
	getAtlasEngine()->getRenderer()->render(getAtlasEngine()->getRenderer()->mCamera);
	return 0;
}

int Atlas::BProject::draw()
{
	getAtlasEngine()->getRenderer()->update();
	texture();
	render();

	return 0;
}

int Atlas::BProject::cleanup(int exitCode) { 
	std::shared_ptr<AtlasEngine> engine = getAtlasEngine();

	engine->getRenderer()->cleanup();
/*	engine->getPhysicsEngine()->cleanup();
	engine->getInputRegistry()->cleanup();
	engine->getLogger()->cleanup();*/

	return exitCode; 
}

std::shared_ptr<BProject> Atlas::BProject::GetProject() {
	
	return mProject;
}

std::shared_ptr<AtlasEngine> Atlas::BProject::getAtlasEngine()
{
	return mAtlas;
}

void Atlas::BProject::ProjectReference::setProjectReference(BProject* project) {
	if (project != nullptr) {
		this->mProject = std::shared_ptr<BProject>(project);
	}
}

//---------------------------------------------------------------------------
//							AtlasEngine
//----------------------------------------------------------------------------

std::shared_ptr<AtlasEngine> Atlas::GetAtlasEngine() { 
	return BProject::GetProject()->getAtlasEngine(); 
}
