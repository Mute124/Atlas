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
	AllocatedPhysicsResources resources = AllocatedPhysicsResources();
	getAtlasEngine()->getPhysicsEngine()->init(resources);
}

void Atlas::BProject::postInit() {
	IWindow* window = getAtlasEngine()->getWindow();
	Renderer* renderer = getAtlasEngine()->getRenderer();

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

Atlas::AtlasEngine::AtlasEngine(ConfigFileRegistry* configFileRegistry, ScriptingAPI* scriptingAPI, IWindow* window, Renderer* renderer,
	GameSettings* gameSettings, FileSystemRegistry* fileSystemRegistry, PhysicsEngine* physicsEngine, InputRegistry* inputRegistry, Logger* logger) {
	setConfigFileRegistry(configFileRegistry);
	setScriptingAPI(scriptingAPI);
	setWindow(window);
	setRenderer(renderer);
	setGameSettings(gameSettings);
	setFileSystemRegistry(fileSystemRegistry);
	setPhysicsEngine(physicsEngine);
	setInputRegistry(inputRegistry);
	setLogger(logger);
}

ConfigFileRegistry* Atlas::AtlasEngine::getConfigFileRegistry()
{
	return configFileRegistry;
}

ScriptingAPI* Atlas::AtlasEngine::getScriptingAPI()
{
	return scriptingAPI;
}

IWindow* Atlas::AtlasEngine::getWindow()
{
	return window;
}

Renderer* Atlas::AtlasEngine::getRenderer()
{
	return renderer;
}

GameSettings* Atlas::AtlasEngine::getGameSettings()
{
	return gameSettings;
}

FileSystemRegistry* Atlas::AtlasEngine::getFileSystemRegistry()
{
	return fileSystemRegistry;
}

PhysicsEngine* Atlas::AtlasEngine::getPhysicsEngine()
{
	return physicsEngine;
}

InputRegistry* Atlas::AtlasEngine::getInputRegistry()
{
	return inputRegistry;
}

Logger* Atlas::AtlasEngine::getLogger()
{
	return logger;
}

void Atlas::AtlasEngine::setConfigFileRegistry(ConfigFileRegistry* configFileRegistry)
{
	ATLAS_GENERATED_NULL_CHECK(configFileRegistry);
	this->configFileRegistry = configFileRegistry;
}

void Atlas::AtlasEngine::setScriptingAPI(ScriptingAPI* scriptingAPI)
{
	ATLAS_GENERATED_NULL_CHECK(scriptingAPI);
	this->scriptingAPI = scriptingAPI;
}

void Atlas::AtlasEngine::setWindow(IWindow* window)
{
	ATLAS_GENERATED_NULL_CHECK(window);
	this->window = window;
}

void Atlas::AtlasEngine::setRenderer(Renderer* renderer)
{
	ATLAS_GENERATED_NULL_CHECK(renderer);
	this->renderer = renderer;
}

void Atlas::AtlasEngine::setGameSettings(GameSettings* gameSettings)
{
	ATLAS_GENERATED_NULL_CHECK(gameSettings);
	this->gameSettings = gameSettings;
}

void Atlas::AtlasEngine::setFileSystemRegistry(FileSystemRegistry* fileSystemRegistry)
{
	ATLAS_GENERATED_NULL_CHECK(fileSystemRegistry);
	this->fileSystemRegistry = fileSystemRegistry;
}

void Atlas::AtlasEngine::setPhysicsEngine(PhysicsEngine* physicsEngine)
{
	ATLAS_GENERATED_NULL_CHECK(physicsEngine);
	this->physicsEngine = physicsEngine;
}

void Atlas::AtlasEngine::setInputRegistry(InputRegistry* inputRegistry)
{
	ATLAS_GENERATED_NULL_CHECK(inputRegistry);
	this->inputRegistry = inputRegistry;
}

void Atlas::AtlasEngine::setLogger(Logger* logger)
{
	ATLAS_GENERATED_NULL_CHECK(logger);
	this->logger = logger;
}

std::shared_ptr<AtlasEngine> Atlas::GetAtlasEngine() { 
	return BProject::GetProject()->getAtlasEngine(); 
}
