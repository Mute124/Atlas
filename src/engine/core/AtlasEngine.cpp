/**************************************************************************************************
 * @file AtlasEngine.cpp
 * 
 * @brief .
 * 
 * @date May 2025
 * 
 * @since v0.0.1
 * 
 * @todo Get rid of the global variables that were used for testing
 ***************************************************************************************************/
#include <iostream>
#include <thread>
#include <cassert>

#include "AtlasEngine.h"

#include "Common.h"
#include "threading/GameThreadScheduler.h"

static volatile bool renderThreadIsDone = false;
static volatile bool updateThreadIsDone = false;
static volatile bool beginLoop = false;


//**
//* @brief Constructs a new AtlasEngine object.
//* @param configFileRegistry A pointer to the config file registry object that Atlas will use
//* @param scriptingAPI A pointer to the scripting API object that Atlas will use
//* @param window A pointer to the window object that Atlas will use
//* @param renderer A pointer to the renderer object that Atlas will use
//* @param gameSettings A pointer to the game settings object that Atlas will use
//* @param fileSystemRegistry A pointer to the file system registry object that Atlas will use
//* @param physicsEngine A pointer to the physics engine object that Atlas will use
//* @param inputRegistry A pointer to the input registry object that Atlas will use
//* @param logger A pointer to the logger object that Atlas will use
//* @since v0.0.8
//*/
//
//Atlas::AtlasEngine::AtlasEngine(ThreadSafeVariable<ConfigFileRegistry*> configFileRegistry, ThreadSafeVariable<ScriptingAPI*> scriptingAPI, ThreadSafeVariable<IWindow*> window, ThreadSafeVariable<Renderer*> renderer, ThreadSafeVariable<GameSettings*> gameSettings, ThreadSafeVariable<FileSystemRegistry*> fileSystemRegistry, ThreadSafeVariable<PhysicsEngine*> physicsEngine, ThreadSafeVariable<InputRegistry*> inputRegistry, ThreadSafeVariable<Logger*> logger)
//{
//	setConfigFileRegistry(configFileRegistry);
//	setScriptingAPI(scriptingAPI);
//	setWindow(window);
//	setRenderer(renderer);
//	setGameSettings(gameSettings);
//	setFileSystemRegistry(fileSystemRegistry);
//	setPhysicsEngine(physicsEngine);
//	setInputRegistry(inputRegistry);
//	setLogger(logger);
//}
//
//Atlas::ThreadSafeVariable<Atlas::ConfigFileRegistry*> Atlas::AtlasEngine::getConfigFileRegistry()
//{
//	return configFileRegistry;
//}
//
//Atlas::ThreadSafeVariable<Atlas::ScriptingAPI*> Atlas::AtlasEngine::getScriptingAPI()
//{
//	return scriptingAPI;
//}
//
//Atlas::ThreadSafeVariable<Atlas::IWindow*> Atlas::AtlasEngine::getWindow()
//{
//	return window;
//}
//
//Atlas::ThreadSafeVariable<Atlas::Renderer*> Atlas::AtlasEngine::getRenderer()
//{
//	return renderer;
//}
//
//Atlas::ThreadSafeVariable<Atlas::GameSettings*> Atlas::AtlasEngine::getGameSettings()
//{
//	return gameSettings;
//}
//
//Atlas::ThreadSafeVariable<Atlas::FileSystemRegistry*> Atlas::AtlasEngine::getFileSystemRegistry()
//{
//	return fileSystemRegistry;
//}
//
//Atlas::ThreadSafeVariable<Atlas::PhysicsEngine*> Atlas::AtlasEngine::getPhysicsEngine()
//{
//	return physicsEngine;
//}
//
//Atlas::ThreadSafeVariable<Atlas::InputRegistry*> Atlas::AtlasEngine::getInputRegistry()
//{
//	return inputRegistry;
//}
//
//Atlas::ThreadSafeVariable<Atlas::Logger*> Atlas::AtlasEngine::getLogger()
//{
//	return logger;
//}
//
//void Atlas::AtlasEngine::setConfigFileRegistry(Atlas::ThreadSafeVariable<ConfigFileRegistry*> configFileRegistry)
//{
//	//ATLAS_GENERATED_NULL_CHECK(configFileRegistry);
//	this->configFileRegistry = configFileRegistry;
//}
//
//void Atlas::AtlasEngine::setScriptingAPI(Atlas::ThreadSafeVariable<ScriptingAPI*> scriptingAPI)
//{
//	//ATLAS_GENERATED_NULL_CHECK(scriptingAPI);
//	this->scriptingAPI = scriptingAPI;
//}
//
//void Atlas::AtlasEngine::setWindow(Atlas::ThreadSafeVariable<IWindow*> window)
//{
//	//ATLAS_GENERATED_NULL_CHECK(window);
//	this->window = window;
//}
//
//void Atlas::AtlasEngine::setRenderer(Atlas::ThreadSafeVariable<Renderer*> renderer)
//{
//	//ATLAS_GENERATED_NULL_CHECK(renderer);
//	this->renderer = renderer;
//}
//
//void Atlas::AtlasEngine::setGameSettings(Atlas::ThreadSafeVariable<GameSettings*> gameSettings)
//{
//	//ATLAS_GENERATED_NULL_CHECK(gameSettings);
//	this->gameSettings = gameSettings;
//}
//
//void Atlas::AtlasEngine::setFileSystemRegistry(Atlas::ThreadSafeVariable<FileSystemRegistry*> fileSystemRegistry)
//{
//	//ATLAS_GENERATED_NULL_CHECK(fileSystemRegistry);
//	this->fileSystemRegistry = fileSystemRegistry;
//}
//
//void Atlas::AtlasEngine::setPhysicsEngine(Atlas::ThreadSafeVariable<PhysicsEngine*> physicsEngine)
//{
//	//ATLAS_GENERATED_NULL_CHECK(physicsEngine);
//	this->physicsEngine = physicsEngine;
//}
//
//void Atlas::AtlasEngine::setInputRegistry(Atlas::ThreadSafeVariable<InputRegistry*> inputRegistry)
//{
//	//ATLAS_GENERATED_NULL_CHECK(inputRegistry);
//	this->inputRegistry = inputRegistry;
//}
//
//void Atlas::AtlasEngine::setLogger(Atlas::ThreadSafeVariable<Logger*> logger)
//{
//	//ATLAS_GENERATED_NULL_CHECK(logger);
//	this->logger = logger;
//}

Atlas::IAtlasEngine::IAtlasEngine(EngineModulesInfo const& modulesInfo, AtlasSettings const& settings)
	: mEngineSettings(settings), mGameRenderingModule(modulesInfo.gameRenderingModule), mGameThreader(modulesInfo.gameThreader)
{
}

void Atlas::AtlasEngine::initWithThreading()
{
	// Make sure everything is good to go before these two modules are used.
	ATLAS_ASSERT(this->mGameRenderingModule != nullptr, "The game rendering module (mGameRenderingModule) in AtlasEngine cannot be a nullptr!");
	ATLAS_ASSERT(this->mGameThreader != nullptr, "The game threading module (mGameThreader) in AtlasEngine cannot be a nullptr!");

	this->mGameThreader->addScheduler("Rendering", 1);
	this->mGameThreader->getScheduler("Rendering")->schedule(
		[&]() {
			runRenderer(); 
		}
	);

	updateThreadIsDone = true;
}

void Atlas::AtlasEngine::initWithoutThreading()
{
	this->runRenderer();
}

void Atlas::AtlasEngine::runRenderer()
{
	this->mGameRenderingModule->init();

	
	this->mGameRenderingModule->mainGameWindow->open((SDL_WindowFlags)(SDL_WINDOW_VULKAN));

	renderThreadIsDone = true;

	// Is all of this required still or is it just a leftover from prototyping?
	while(!beginLoop) {
		// Simply idle until the threads are done
	}


	while (true) {
		this->mGameRenderingModule->update();
	}
}

void Atlas::AtlasEngine::init()
{

	if (this->mEngineSettings.isThreaded) {
		initWithThreading();
	}
	else {
		initWithoutThreading();
	}
}

void Atlas::AtlasEngine::run()
{


	while (!renderThreadIsDone || !updateThreadIsDone) {

	}

	beginLoop = true;

	while (!this->mGameRenderingModule->shouldExit()) {
		this->mGameRenderingModule->update();
	}
}

void Atlas::AtlasEngine::cleanup()
{
}
