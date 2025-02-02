#include "AtlasEngine.h"
#pragma once

Atlas::AtlasEngine::AtlasEngine(ConfigFileRegistry* configFileRegistry, ScriptingAPI* scriptingAPI, IWindow* window, Renderer* renderer, GameSettings* gameSettings, FileSystemRegistry* fileSystemRegistry, PhysicsEngine* physicsEngine, InputRegistry* inputRegistry, Logger* logger) {
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

Atlas::ConfigFileRegistry* Atlas::AtlasEngine::getConfigFileRegistry()
{
	return configFileRegistry;
}

Atlas::ScriptingAPI* Atlas::AtlasEngine::getScriptingAPI()
{
	return scriptingAPI;
}

Atlas::IWindow* Atlas::AtlasEngine::getWindow()
{
	return window;
}

Atlas::Renderer* Atlas::AtlasEngine::getRenderer()
{
	return renderer;
}

Atlas::GameSettings* Atlas::AtlasEngine::getGameSettings()
{
	return gameSettings;
}

Atlas::FileSystemRegistry* Atlas::AtlasEngine::getFileSystemRegistry()
{
	return fileSystemRegistry;
}

Atlas::PhysicsEngine* Atlas::AtlasEngine::getPhysicsEngine()
{
	return physicsEngine;
}

Atlas::InputRegistry* Atlas::AtlasEngine::getInputRegistry()
{
	return inputRegistry;
}

Atlas::Logger* Atlas::AtlasEngine::getLogger()
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
