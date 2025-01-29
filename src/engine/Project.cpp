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
			new InputRegistry()
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
	Logger::Instance().init(LoggerConfig());

	AddFileRegistryLoadFunction("png", [](std::shared_ptr<FileMeta> loadFunc) {
		return std::any_cast<Image>(LoadImage(loadFunc->path.c_str()));
		});

	AddFileRegistryLoadFunction("jpg", [](std::shared_ptr<FileMeta> loadFunc) {
		return std::any_cast<Image>(LoadImage(loadFunc->path.c_str()));
		});

	AddFileRegistryLoadFunction("jpeg", [](std::shared_ptr<FileMeta> loadFunc) {
		return std::any_cast<Image>(LoadImage(loadFunc->path.c_str()));
		});

	std::string gameDir = ATLAS_GAME_DIR;

	getAtlasEngine()->getFileSystemRegistry()->init(gameDir.c_str());
	getAtlasEngine()->getConfigFileRegistry()->init();

//#ifdef ATLAS_ENABLE_MODDDING
//		#ifdef ATLAS_ENABLE_LUA
	this->mLuaLibraries.push_back(sol::lib::base);
	getAtlasEngine()->getScriptingAPI()->initializeScripting(this->getLuaLibraries(), this->getLuaFunctions()); // getAtlasEngine()->getLuaLibraries(), getAtlasEngine()->getLuaFunctions()
	getAtlasEngine()->getScriptingAPI()->registerLua();
//		#endif
//#endif
}

void Atlas::BProject::init(int argc, char* argv[]) {
	AllocatedPhysicsResources resources = AllocatedPhysicsResources();
	getAtlasEngine()->getPhysicsEngine()->init(resources);
}

void Atlas::BProject::postInit() {
	getAtlasEngine()->getWindow()->init(new WindowDecorations());
	getAtlasEngine()->getRenderer()->init();
}

void Atlas::BProject::initRenderer() {
}

int Atlas::BProject::run(int argc, char* argv[]) {
	int code = 0;
	
	while (!getAtlasEngine()->getWindow()->shouldClose()) {
		
		draw();
	}
	std::this_thread::yield();
	return code;
}


int Atlas::BProject::update()
{
	int code = 0;

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
	PhysicsEngine::Instance().update(1.0f / 60.0f);
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
	GameSettings* gameSettings, FileSystemRegistry* fileSystemRegistry, PhysicsEngine* physicsEngine, InputRegistry* inputRegistry) {
	setConfigFileRegistry(configFileRegistry);
	setScriptingAPI(scriptingAPI);
	setWindow(window);
	setRenderer(renderer);
	setGameSettings(gameSettings);
	setFileSystemRegistry(fileSystemRegistry);
	setPhysicsEngine(physicsEngine);
	setInputRegistry(inputRegistry);
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

void Atlas::AtlasEngine::setConfigFileRegistry(ConfigFileRegistry* configFileRegistry)
{
	this->configFileRegistry = configFileRegistry;
}

void Atlas::AtlasEngine::setScriptingAPI(ScriptingAPI* scriptingAPI)
{
	this->scriptingAPI = scriptingAPI;
}

void Atlas::AtlasEngine::setWindow(IWindow* window)
{
	this->window = window;
}

void Atlas::AtlasEngine::setRenderer(Renderer* renderer)
{
	this->renderer = renderer;
}

void Atlas::AtlasEngine::setGameSettings(GameSettings* gameSettings)
{
	this->gameSettings = gameSettings;
}

void Atlas::AtlasEngine::setFileSystemRegistry(FileSystemRegistry* fileSystemRegistry)
{
	this->fileSystemRegistry = fileSystemRegistry;
}

void Atlas::AtlasEngine::setPhysicsEngine(PhysicsEngine* physicsEngine)
{
	this->physicsEngine = physicsEngine;
}

void Atlas::AtlasEngine::setInputRegistry(InputRegistry* inputRegistry)
{
	this->inputRegistry = inputRegistry;
}

std::shared_ptr<AtlasEngine> Atlas::GetAtlasEngine() { 
	return BProject::GetProject()->getAtlasEngine(); 
}
